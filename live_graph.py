#importing libraries
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import json

#creating a subplot 


fig = plt.figure(figsize=plt.figaspect(1))
j = 0
def animate(i):
    global j
    
    data_arr = np.random.randint(2, size=25)
    
#it is an array of size 1x(columns*rows)

    rows = 5
    columns = 5
    
    Z = data_arr.reshape(columns,rows)

    ax = fig.add_subplot(1,1,1)

    plt.imshow(Z,origin='lower',interpolation='bilinear', cmap='autumn',extent = [0,2*rows,0,2*columns])
    plt.colorbar()
    
	

ani = animation.FuncAnimation(fig, animate, interval=30) 
ani.save('orbita.gif', writer='imagemagick', fps=30)
plt.show()