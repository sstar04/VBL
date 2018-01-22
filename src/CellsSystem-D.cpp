/*
 *  CellsSystem-D-3.cpp
 *  Sim3D
 *
 *  Created by Edoardo Milotti on 09/01/17.
 *
 */

#include "CellsSystem.h"

using namespace vbl;

// ************ this section contains the interface to CGAL ************ //
// 
// REMARK: the CGAL methods are used ONLY in this part of the program, and are isolated 
// from the rest of the code to avoid interferences.
//
void CellsSystem::Geometry()
{
  unsigned long k;
  // cout << "Triangulation OK" << endl;

  // insertion of cells' centres into the vector of Point structures
  // the vector of Points is defined in CellsSystem.h, and the initial reserve is set in 
  // CellsSystem.cpp

  v.clear(); // vector of all points
  arr_of_vn_pointers.clear();
  arr_of_vn_pointers.resize(ncells);

  for(k=0; k<ncells; k++)
    v.push_back( std::make_pair(Point(x[k],y[k],z[k]),k) ); 
	
	// cout << "v size: " << v.size() << endl;

	// cout << "Setup punti OK" << endl;
	// cout << "Primo punto: {" << v[0] << "} = " << x[0] << ", " << y[0] << ", " << z[0] << endl;

//Delaunay triangulation
  Triangulation_3 DelTri( v.begin(),v.end() );

// next we find the list of connections and we estimate contact areas

	//std::vector<Vertex_handle> vn; // vector of Vertex_handle's of neighbors

// loop over finite vertices (k is the name of current cell)
  for (Finite_vertices_iterator vit = DelTri.finite_vertices_begin(); vit != DelTri.finite_vertices_end(); ++vit)
    {
      k = vit->info();
      arr_of_vn_pointers[k] = std::shared_ptr<std::vector<Vertex_handle>>(new std::vector<Vertex_handle>);
      DelTri.incident_vertices(vit, std::back_inserter(*arr_of_vn_pointers[k])); // list of neighbors
      //DelTri.incident_vertices(vit, back_inserter(vn));	// list of neighbors
    }
//#pragma omp parallel for
/* each thread needs its own neighbor array! */
  std::array<int,MAX_N_NEIGH> neigh_per_thread;					// numero di vicini
  std::array< std::vector<int>, MAX_N_NEIGH > vneigh_per_thread;		// vettore dei vicini
  std::array< std::vector<double>, MAX_N_NEIGH > vdist_per_thread;     // vettore delle distanze dai vicini
  std::array< std::vector<double>, MAX_N_NEIGH > vcsurf_per_thread;	// vettore delle superfici di contatto con i vicini (calcolo approx)
  std::array< std::vector<double>, MAX_N_NEIGH> gnk_per_thread;		// vettore dei fattori geometrici
  std::array<double, MAX_N_NEIGH> contact_surf_per_thread;
  for( k = 0;k<ncells; ++k)
  {
    //neigh[k] = vn.size();								// number of all neighbors
    neigh[k] = (*arr_of_vn_pointers[k]).size();
    vneigh[k].resize(neigh[k]);							// init names of neighbors
    vcsurf[k].resize(neigh[k]);							// init contact areas

    vdist[k].resize(neigh[k]);							// allocate distance vect
    gnk[k].resize(neigh[k]);							// allocate geom factors
    isonCH[k]=false;									// bool true if ON convex 
												      // hull
    env_surf[k] = 0.;									// contact area with 
												      // environment
    g_env[k] = 0.;										// geom factor with 
												      // environment

    // weighted radius of current cell
    double rk = (type[k]->Get_extension_coeff())*r[k];

    contact_surf[k] = 0.;								// init total contact area

    int nFV = 0;										// init number of finite 
												      // vertices

    // in this loop we prepare the list of neighbors and we compute contact areas 
    if(neigh[k] > 0)
    {
      for(int kk=0; kk < neigh[k] ; kk++)					
      {
	if(!DelTri.is_infinite((*arr_of_vn_pointers[k])[kk]))	// if true, then there is a neighboring 
	     // finite vertex
	{  
	  int neighbor = (*arr_of_vn_pointers[k])[kk]->info(); // name of neighbor
	  vneigh[k][nFV] = neighbor; // store name of neighbor
	  //***
	  // weighted radius of neighboring cell
	  double rkk = (type[neighbor]->Get_extension_coeff())*r[neighbor];	
	  double dd = Distance( k,neighbor ); // distance between current cell and neigbor
	  // *********** check for debugging
	  if(dd != dd || dd <= 0)
	  {
	    std::cout << k << "-th cell, neighbor " << neighbor << ", undefined distance " << dd << std::endl;
	  }
		// *********** end check for debugging
		
	  vdist[k][nFV] = dd;
	  
	  if( dd < (rk+rkk) ) // here we compute the contact area
	  {
	    vcsurf[k][nFV] = -PI*(SQR(dd)-SQR(rk-rkk))*(SQR(dd)-SQR(rk+rkk))/(4*SQR(dd));
	    if( vcsurf[k][nFV] < 0 ) vcsurf[k][nFV] = 0;
	    contact_surf[k] += vcsurf[k][nFV];
	  }
	  else
	  {
	    vcsurf[k][nFV] = 0.;
	  }

	  if(vcsurf[k][nFV] > 0)						// geometric factor
	  {
		  gnk[k][nFV] = vcsurf[k][nFV]/dd;
	  }
	  else
	  {
		  gnk[k][nFV] = 0;
	  }
	      //***
	nFV++;	// here we increase the counter of finite vertices
	}
	else
	{
	  isonCH[k]=true;	// if any one of the adjacent vertices is infinite 
						// then cell is on convex hull
	}
      }
    }//end if if(neigh[k] > 0)


    if( nFV != neigh[k] )	// qui si controlla il numero di vertici finiti e se questo e' diverso dal numero totale di vertici
						// si fa un resize dei vettori
    {
      neigh[k] = nFV;
      vneigh[k].resize(neigh[k]);						// resizing del vettore dei nomi dei vicini
      vcsurf[k].resize(neigh[k]);						// resizing del vettore delle aree delle superfici di contatto
      vdist[k].resize(neigh[k]);						// resizing del vettore delle distanze
      gnk[k].resize(neigh[k]);						// resizing del vettore dei fattori geometrici
    }

    // *** inizio calcolo fattore geometrico ambientale
    // calcolo della superficie di contatto con l'ambiente: si assume che tutta la superficie della cellula che non e' a
    // contatto con le cellule adiacenti sia in contatto con l'ambiente: questo calcolo viene fatto comunque, ma il 
    // contatto con l'ambiente c'e' in realta' solo le la cellula sta sull'alpha shape

    env_surf[k] = 0.;								// normalmente la sup. di contatto con l'ambiente e' nulla
    g_env[k] = 0.;									// e naturalmente anche il fattore geometrico associato e' nullo


    //			env_surf[k] = surface[k] - contact_surf[k];		// qui si calcola la superficie esposta all'ambiente
    //			if( env_surf[k] < 0 )
    //				env_surf[k] = 0.;							// in linea di principio (causa algoritmo approssimato) questa superficie esposta 
												// puo' essere negativa, e in questo caso la si annulla


    // nuovo calcolo della superficie esposta all'ambiente: si calcola il numero di vicini + l'ambiente e si assume che la superficie surface[k]
    // sia equamente suddivisa tra vicini e ambiente
    env_surf[k] = surface[k]/(neigh[k]+1);			// qui si calcola la superficie esposta all'ambiente

    g_env[k] = env_surf[k]/r[k];					// fattore geometrico verso l'ambiente

    // *** fine del calcolo del fattore geometrico con l'ambiente


    //vn.clear();											// si ripulisce la lista dei vicini in preparazione del prossimo vertice
    arr_of_vn_pointers[k]->clear();
  }// end for( k = 0;k<ncells; ++k) end PARALLEL 

  // compute fixed alpha shape with ALPHAVALUE defined in sim.h

  if( ncells < 5 )	// if there are less than 5 cells they are all on alphashape
  {
    for(k=0; k<ncells; k++)
    {
      isonAS[k] = true;
    }
  }
  else 
  {
    Fixed_alpha_shape_3 as(v.begin(),v.end(),ALPHAVALUE);
    std::vector<Vertex_handle> vertices_on_alpha_shape;
    as.get_alpha_shape_vertices(std::back_inserter(vertices_on_alpha_shape),Fixed_alpha_shape_3::REGULAR);
    as.get_alpha_shape_vertices(std::back_inserter(vertices_on_alpha_shape),Fixed_alpha_shape_3::SINGULAR);
    // cout << "there are " << vertices_on_alpha_shape.size() << " vertices on alpha shape " << endl;
    // initialize boolean indicator variable
    for(k=0; k<ncells; k++)
    {
	  isonAS[k] = false;
    }
    // set boolean true for vertices on alpha shape
    for(k=0; k<vertices_on_alpha_shape.size(); k++)
    {
      isonAS[vertices_on_alpha_shape[k]->info()] = true;
    }
  }
  // questo loop azzera i g_env di tutte le cellule che non stanno sull'alpha shape
  for(k=0; k<ncells; k++)
  {
    if( !isonAS[k] ) g_env[k]=0.;
  }
    
  // the following loop identifies those cells that are in contact with blood vessels 
  for(k=0; k<ncells; k++)
  {    
    std::array<double,3> cellpos; // store the cell coordinates in a 3-vector
    cellpos[0]=x[k];
    cellpos[1]=y[k];
    cellpos[2]=z[k];

    isonBV[k] = 0; // by default, cells are not close to blood vessels 
    g_bv[k] = 0; // by default, there is no contact term with blood vessels
    
    for( int nvessel=0; nvessel<nbv; nvessel++ ) // loop over all blood vessels
    {
      double x0[3]; // position on blood vessel axis closest to cell
      double dbv = BloodVesselVector[nvessel].DistanceFromVessel( cellpos, x0 ); // distance between cell and blood vessel
      if( dbv < BloodVesselVector[nvessel].GetBloodVesselR() + r[k] ) // if the cell's center and the blood vessel axis are closer than the sum of the radii, then there is contact
      {
	isonBV[k] = nvessel+1;		// note the shift, which is done to use the value 0 as false and otherwise use the true value to store the blood vessel position in the blood vessel vector
	bv_surf[k] = surface[k];
	g_bv[k] = bv_surf[k]/r[k]; // this are bold statements; here we assume that the cell behaves like a disk (not a sphere), and that half of the surface area faces the blood vessel
	// the last approximation should probably be mitigated with the inclusion of a surface-modulating parameter or even better by an improved geometrical modelling
	break;	// blood vessel found, we jump out of the blood vessel loop	
      }
    }
    // g_bv[k] = 0; // **** TEMPORARY, used only to eliminate blood vessels from calculations !!!   
  }
}

// calcoli minimi nel caso di cellule disperse
void CellsSystem::NoGeometry()
{

	for(unsigned long k=0; k<ncells; k++)
		{
		env_surf[k] = surface[k];				// qui si calcola la superficie esposta all'ambiente
		g_env[k] = env_surf[k]/r[k];			// fattore geometrico verso l'ambiente
		contact_surf[k] = 0;					// la sup. di contatto con le altre cellule e' nulla nel caso di cellule disperse
		g_bv[k] = 0;							// no contact with blood vessels
		}


}
