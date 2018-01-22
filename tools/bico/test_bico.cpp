
#include <iostream>
#include <cmath> // log exp sin tan floor

using namespace std;

double gammln(const double xx)
{
	int j;
	double x,y,tmp,ser;
	static const double cof[6]={76.18009172947146,-86.50532032941677,
		24.01409824083091,-1.231739572450155,0.1208650973866179e-2,
		-0.5395239384953e-5};

	y=x=xx;
	tmp=x+5.5;
	tmp -= (x+0.5)*log(tmp);
	ser=1.000000000190015;
	for (j=0;j<6;j++) ser += cof[j]/++y;
	return -tmp+log(2.5066282746310005*ser/x);
}

//log of factorial
double factln(const int n)
{
	static double a[101];

	if (n < 0) 
		std::cout << "Negative factorial in routine factln";
	if (n <= 1) 
		return 0.0;
	if (n <= 100)
		return (a[n] != 0.0 ? a[n] : (a[n]=gammln(n+1.0)));
	else 
		return gammln(n+1.0);
}

double factln_fast(const int n)
{
	static double a[1000] = {0,0,0.693147,1.79176,3.17805,4.78749,6.57925,8.52516,10.6046,12.8018,15.1044,17.5023,19.9872,22.5522,25.1912,27.8993,30.6719,33.5051,36.3954,39.3399,
    42.3356,45.3801,48.4712,51.6067,54.7847,58.0036,61.2617,64.5575,67.8897,71.257,74.6582,78.0922,81.558,85.0545,88.5808,92.1362,95.7197,99.3306,102.968,
    106.632,110.321,114.034,117.772,121.533,125.317,129.124,132.953,136.803,140.674,144.566,148.478,152.41,156.361,160.331,164.32,168.327,172.353,176.396,
    180.456,184.534,188.628,192.739,196.866,201.009,205.168,209.343,213.532,217.737,221.956,226.191,230.439,234.702,238.978,243.269,247.573,251.89,256.221,
    260.565,264.922,269.291,273.673,278.068,282.474,286.893,291.324,295.767,300.221,304.687,309.164,313.653,318.153,322.663,327.185,331.718,336.261,340.815,
    345.379,349.954,354.539,359.134,363.739,368.354,372.979,377.614,382.259,386.913,391.576,396.249,400.931,405.622,410.323,415.032,419.751,424.478,429.214,
    433.959,438.713,443.475,448.246,453.025,457.812,462.608,467.412,472.224,477.045,481.873,486.709,491.553,496.405,501.265,506.133,511.008,515.891,520.781,
    525.679,530.584,535.497,540.417,545.344,550.279,555.22,560.169,565.125,570.088,575.058,580.034,585.018,590.008,595.006,600.009,605.02,610.037,615.061,
    620.092,625.129,630.172,635.222,640.278,645.341,650.41,655.485,660.566,665.654,670.748,675.847,680.953,686.065,691.183,696.307,701.437,706.573,711.715,
    716.862,722.016,727.175,732.339,737.51,742.686,747.868,753.055,758.248,763.447,768.651,773.86,779.075,784.295,789.521,794.752,799.989,805.23,810.477,
    815.73,820.987,826.25,831.518,836.791,842.069,847.352,852.64,857.934,863.232,868.535,873.844,879.157,884.475,889.798,895.126,900.458,905.796,911.138,
    916.485,921.837,927.194,932.555,937.921,943.292,948.667,954.047,959.431,964.821,970.214,975.612,981.015,986.422,991.834,997.25,1002.67,1008.1,1013.52,
    1018.96,1024.4,1029.84,1035.29,1040.74,1046.19,1051.65,1057.12,1062.58,1068.06,1073.53,1079.01,1084.5,1089.99,1095.48,1100.98,1106.48,1111.98,1117.49,
    1123.01,1128.52,1134.05,1139.57,1145.1,1150.63,1156.17,1161.71,1167.26,1172.81,1178.36,1183.92,1189.48,1195.04,1200.61,1206.18,1211.76,1217.34,1222.92,
    1228.51,1234.1,1239.69,1245.29,1250.89,1256.5,1262.11,1267.72,1273.34,1278.96,1284.58,1290.21,1295.84,1301.48,1307.12,1312.76,1318.4,1324.05,1329.7,
    1335.36,1341.02,1346.68,1352.35,1358.02,1363.69,1369.37,1375.05,1380.73,1386.42,1392.11,1397.8,1403.5,1409.2,1414.91,1420.61,1426.32,1432.04,1437.75,
    1443.47,1449.2,1454.92,1460.65,1466.39,1472.12,1477.86,1483.61,1489.35,1495.1,1500.86,1506.61,1512.37,1518.13,1523.9,1529.67,1535.44,1541.21,1546.99,
    1552.77,1558.55,1564.34,1570.13,1575.92,1581.72,1587.52,1593.32,1599.13,1604.93,1610.75,1616.56,1622.38,1628.2,1634.02,1639.85,1645.68,1651.51,1657.34,
    1663.18,1669.02,1674.86,1680.71,1686.56,1692.41,1698.27,1704.12,1709.99,1715.85,1721.72,1727.58,1733.46,1739.33,1745.21,1751.09,1756.97,1762.86,
    1768.75,1774.64,1780.53,1786.43,1792.33,1798.23,1804.14,1810.05,1815.96,1821.87,1827.79,1833.71,1839.63,1845.55,1851.48,1857.41,1863.34,1869.28,
    875.21,1881.15,1887.1,1893.04,1898.99,1904.94,1910.89,1916.85,1922.81,1928.77,1934.73,1940.7,1946.67,1952.64,1958.61,1964.59,1970.57,1976.55,1982.53,
    1988.52,1994.51,2000.5,2006.49,2012.49,2018.49,2024.49,2030.5,2036.5,2042.51,2048.52,2054.54,2060.55,2066.57,2072.59,2078.61,2084.64,2090.67,2096.7,
    2102.73,2108.77,2114.81,2120.85,2126.89,2132.93,2138.98,2145.03,2151.08,2157.14,2163.19,2169.25,2175.31,2181.38,2187.44,2193.51,2199.58,2205.66,
    2211.73,2217.81,2223.89,2229.97,2236.06,2242.14,2248.23,2254.32,2260.42,2266.51,2272.61,2278.71,2284.81,2290.92,2297.03,2303.14,2309.25,2315.36,
    2321.48,2327.59,2333.71,2339.84,2345.96,2352.09,2358.22,2364.35,2370.48,2376.62,2382.76,2388.9,2395.04,2401.18,2407.33,2413.48,2419.63,2425.78,
    2431.93,2438.09,2444.25,2450.41,2456.58,2462.74,2468.91,2475.08,2481.25,2487.42,2493.6,2499.78,2505.96,2512.14,2518.32,2524.51,2530.7,2536.89,
    2543.08,2549.27,2555.47,2561.67,2567.87,2574.07,2580.28,2586.48,2592.69,2598.9,2605.12,2611.33,2617.55,2623.77,2629.99,2636.21,2642.43,2648.66,
    2654.89,2661.12,2667.35,2673.59,2679.82,2686.06,2692.3,2698.54,2704.79,2711.03,2717.28,2723.53,2729.78,2736.04,2742.29,2748.55,2754.81,2761.07,
    2767.33,2773.6,2779.87,2786.14,2792.41,2798.68,2804.96,2811.23,2817.51,2823.79,2830.07,2836.36,2842.64,2848.93,2855.22,2861.51,2867.81,2874.1,
    2880.4,2886.7,2893,2899.3,2905.6,2911.91,2918.22,2924.53,2930.84,2937.15,2943.47,2949.79,2956.11,2962.43,2968.75,2975.07,2981.4,2987.73,2994.06,
    3000.39,3006.72,3013.06,3019.39,3025.73,3032.07,3038.42,3044.76,3051.1,3057.45,3063.8,3070.15,3076.5,3082.86,3089.22,3095.57,3101.93,3108.29,
    3114.66,3121.02,3127.39,3133.76,3140.13,3146.5,3152.87,3159.25,3165.62,3172,3178.38,3184.76,3191.15,3197.53,3203.92,3210.31,3216.7,3223.09,
    3229.48,3235.88,3242.28,3248.67,3255.07,3261.48,3267.88,3274.28,3280.69,3287.1,3293.51,3299.92,3306.34,3312.75,3319.17,3325.59,3332.01,3338.43,
    3344.85,3351.28,3357.7,3364.13,3370.56,3376.99,3383.42,3389.86,3396.29,3402.73,3409.17,3415.61,3422.06,3428.5,3434.95,3441.39,3447.84,3454.29,
    3460.74,3467.2,3473.65,3480.11,3486.57,3493.03,3499.49,3505.95,3512.42,3518.88,3525.35,3531.82,3538.29,3544.76,3551.24,3557.71,3564.19,3570.67,
    3577.15,3583.63,3590.11,3596.6,3603.08,3609.57,3616.06,3622.55,3629.04,3635.54,3642.03,3648.53,3655.03,3661.53,3668.03,3674.53,3681.04,3687.54,
    3694.05,3700.56,3707.07,3713.58,3720.09,3726.61,3733.12,3739.64,3746.16,3752.68,3759.2,3765.73,3772.25,3778.78,3785.31,3791.84,3798.37,3804.9,
    3811.43,3817.97,3824.5,3831.04,3837.58,3844.12,3850.67,3857.21,3863.76,3870.3,3876.85,3883.4,3889.95,3896.5,3903.06,3909.61,3916.17,3922.73,
    3929.29,3935.85,3942.41,3948.97,3955.54,3962.11,3968.67,3975.24,3981.81,3988.39,3994.96,4001.54,4008.11,4014.69,4021.27,4027.85,4034.43,4041.02,
    4047.6,4054.19,4060.77,4067.36,4073.95,4080.55,4087.14,4093.73,4100.33,4106.93,4113.52,4120.12,4126.73,4133.33,4139.93,4146.54,4153.14,4159.75,
    4166.36,4172.97,4179.58,4186.2,4192.81,4199.43,4206.05,4212.66,4219.28,4225.91,4232.53,4239.15,4245.78,4252.4,4259.03,4265.66,4272.29,4278.92,
    4285.56,4292.19,4298.83,4305.47,4312.1,4318.74,4325.39,4332.03,4338.67,4345.32,4351.96,4358.61,4365.26,4371.91,4378.56,4385.21,4391.87,4398.52,
    4405.18,4411.84,4418.5,4425.16,4431.82,4438.48,4445.15,4451.81,4458.48,4465.15,4471.82,4478.49,4485.16,4491.83,4498.51,4505.18,4511.86,4518.54,
    4525.22,4531.9,4538.58,4545.27,4551.95,4558.64,4565.32,4572.01,4578.7,4585.39,4592.08,4598.78,4605.47,4612.17,4618.87,4625.56,4632.26,4638.96,
    4645.67,4652.37,4659.07,4665.78,4672.49,4679.19,4685.9,4692.61,4699.33,4706.04,4712.75,4719.47,4726.18,4732.9,4739.62,4746.34,4753.06,4759.79,
    4766.51,4773.23,4779.96,4786.69,4793.42,4800.15,4806.88,4813.61,4820.34,4827.08,4833.81,4840.55,4847.29,4854.03,4860.77,4867.51,4874.25,4881,
    4887.74,4894.49,4901.24,4907.99,4914.74,4921.49,4928.24,4934.99,4941.75,4948.5,4955.26,4962.02,4968.78,4975.54,4982.3,4989.06,4995.83,5002.59,
    5009.36,5016.12,5022.89,5029.66,5036.43,5043.2,5049.98,5056.75,5063.53,5070.3,5077.08,5083.86,5090.64,5097.42,5104.2,5110.99,5117.77,5124.56,
    5131.34,5138.13,5144.92,5151.71,5158.5,5165.29,5172.09,5178.88,5185.68,5192.47,5199.27,5206.07,5212.87,5219.67,5226.48,5233.28,5240.08,5246.89,
    5253.7,5260.5,5267.31,5274.12,5280.93,5287.75,5294.56,5301.37,5308.19,5315.01,5321.83,5328.64,5335.46,5342.29,5349.11,5355.93,5362.75,5369.58,
    5376.41,5383.23,5390.06,5396.89,5403.72,5410.56,5417.39,5424.22,5431.06,5437.89,5444.73,5451.57,5458.41,5465.25,5472.09,5478.93,5485.78,5492.62,
    5499.47,5506.32,5513.16,5520.01,5526.86,5533.71,5540.57,5547.42,5554.27,5561.13,5567.99,5574.84,5581.7,5588.56,5595.42,5602.28,5609.15,5616.01,
    5622.88,5629.74,5636.61,5643.48,5650.35,5657.22,5664.09,5670.96,5677.83,5684.71,5691.58,5698.46,5705.33,5712.21,5719.09,5725.97,5732.85,5739.74,
    5746.62,5753.5,5760.39,5767.28,5774.16,5781.05,5787.94,5794.83,5801.72,5808.62,5815.51,5822.41,5829.3,5836.2,5843.1,5849.99,5856.89,5863.79,
    5870.7,5877.6,5884.5,5891.41,5898.31,5905.22};

	if (n < 0) 
		std::cout << "Negative factorial in routine factln";
	if (n <= 1) 
		return 0.0;
	if (n < 10)
		return a[n];
	else 
		return gammln(n+1.0);
}

//binomial coefficient
double bico(const int n, const int k)
{
	return floor(0.5+exp(factln(n)-factln(k)-factln(n-k)));
}
int main() 
{
    cout << "Testing Binomials!!"<< endl;
    for( int n = 5;n<10; n++)
    {
      for( int k =1; k<4; k++)
      {
        cout << "(n,k): " << "( " << n << ","<<k <<") " << bico(n,k) << endl;
      }
    }
    cout << "FAKTLN" << endl;
    for( int i=0;i<1000;i++)
    {
      cout << factln(i) << "," ;
    }
    cout << endl << "FAKTLN_FAST" << endl;
    for( int i=0;i<1000;i++)
    {
      cout << factln_fast(i) << "," ;
    }
    cout << endl;
    return 0;
}
