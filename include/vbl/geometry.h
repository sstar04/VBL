#ifndef GEOMETRY_H
#define GEOMETRY_H  // header guard
// header for the geometry part
#include <CGAL/Default.h>

#ifdef CGAL_LINKED_WITH_TBB
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>
#include <CGAL/Fixed_alpha_shape_3.h>
#include <CGAL/Fixed_alpha_shape_vertex_base_3.h>
#include <CGAL/Fixed_alpha_shape_cell_base_3.h>
  
typedef CGAL::Exact_predicates_inexact_constructions_kernel     	K;

typedef CGAL::Triangulation_vertex_base_with_info_3<unsigned, K> 	Vb;
typedef CGAL::Fixed_alpha_shape_vertex_base_3<K, Vb>      			AsVb;
typedef CGAL::Fixed_alpha_shape_cell_base_3<K>        				Fb;
typedef CGAL::Triangulation_data_structure_3<AsVb,Fb,CGAL::Parallel_tag>               Tds;
typedef CGAL::Delaunay_triangulation_3<K,Tds>                      	Triangulation_3;
typedef CGAL::Fixed_alpha_shape_3<Triangulation_3>					Fixed_alpha_shape_3;

// ****************

typedef K::Point_3                                          Point;
typedef Fixed_alpha_shape_3::Vertex_handle					Vertex_handle;
typedef Triangulation_3::Finite_vertices_iterator			Finite_vertices_iterator;

static std::vector< std::pair<Point,unsigned> > v;	// vector of points with info passed to CGAL
static std::vector<std::shared_ptr<std::vector<Vertex_handle>>> arr_of_vn_pointers; // vector of Vertex_handle's of neighbors

#endif

#endif //#ifndef GEOM2_H
