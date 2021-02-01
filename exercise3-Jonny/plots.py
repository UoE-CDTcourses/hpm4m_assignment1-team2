import numpy as np
import matplotlib.pyplot as plt

## Standard
# Load variables
errors = np.loadtxt('errors.txt')
powers = np.loadtxt('powers.txt')
gridsize = (powers + 1)**2

print(gridsize)
# Find convergence rate
rate = -np.polyfit(np.log10(gridsize[5:]), np.log10(errors[5:]), deg = 1)[0]
# Fit line
const = errors[-1]*(gridsize[-1]**(rate))
G3 = (1e3*const)**(1/rate)
Gmachine  = (1e16*const)**(1/rate)
print('machine error: ', Gmachine)
print('final err: ', errors[-1])
ns = np.array([2, 10*gridsize[-1]])
print('const', const, 'rate', rate)
# Plot
fs = 12 # Fontsize
fig, ax = plt.subplots(1, 1, figsize = (6, 6))
ax.plot(gridsize, errors, 'r-*')
ax.plot(ns, const*ns**(-rate), 'k--')
ax.plot([1, G3], [1e-3, 1e-3], 'b--')
ax.plot([G3, G3], [1e-5, 1e-3], 'b--')
ax.text(G3/10000, 1e-4, 'Grid size = ' + str(int(G3)))
ax.set_xscale('log', base = 10)
ax.set_yscale('log')
ax.set_title('Rate = ' + str(rate)[:4])
ax.set_xlabel(r'Grid Size', fontsize = fs)
ax.set_ylabel('Error', fontsize = fs)
fig.savefig('err.eps')


# Plot time vs cores

# Rectangular array
fig2, ax2 = plt.subplots(1, 1, figsize = (6, 6))
cores = [1, 2, 4, 8, 16, 32]
times = [119314381, 61703990, 30492597, 15970691, 7653018, 6038761]
ax2.plot(cores, times, 'r-*', label = 'Rectangular split')
cores_sq = [1, 4, 9, 16, 25]
times_sq = [119279369, 31097550, 13784843, 8662394, 6621792]
ax2.plot(cores_sq, times_sq, 'b-*', label = 'Square split')
ax2.set_xlabel('No. cores', fontsize = fs)
ax2.set_ylabel('Runtime', fontsize = fs)
ax2.set_xscale('log', base = 2), ax2.set_yscale('log', base = 2)
fig2.legend(fontsize = fs)
fig2.savefig('cputime_rect.eps')
