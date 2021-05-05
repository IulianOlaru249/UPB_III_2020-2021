#Source: https://www.geeksforgeeks.org/graph-plotting-in-python-set-1/

import matplotlib.pyplot as plt
  
# x axis values
neopt_y = [0.029485,
0.163215,
0.432635,
1.041117,
3.516554,
5.611711,
8.437189,
16.108234,
28.163424,
76.199486]

opt_y =[
0.004197,
0.057072,
0.147125,
0.276927,
0.973798,
1.719783,
2.214869,
4.257677,
7.444541,
21.773569]

blas_y = [
0.001497,
0.005292,
0.016226,
0.035308,
0.112777,
0.177244,
0.291450,
0.496417,
0.846135,
1.982621]

# corresponding y axis values
x = [100, 200, 300, 400, 600, 700, 800, 1000, 1200, 1600]
  
plt.vlines(x, 0, neopt_y, linestyle="dashed")
# plt.hlines(neopt_y, 0, x, linestyle="dashed")
plt.plot(x, neopt_y, label = "neopt")

plt.vlines(x, 0, opt_y, linestyle="dashed")
# plt.hlines(neopt_y, 0, x, linestyle="dashed")
plt.plot(x, opt_y, label = "opt")

plt.vlines(x, 0, blas_y, linestyle="dashed")
# plt.hlines(blas_y, 0, x, linestyle="dashed")
plt.plot(x, blas_y, label = "blas")
  
plt.xlabel('Time')
plt.ylabel('N')
  
plt.legend()
plt.title('Performance Analysis')
plt.show()