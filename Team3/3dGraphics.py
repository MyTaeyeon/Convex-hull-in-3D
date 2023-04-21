from mpl_toolkits import mplot3d
import numpy as np
import matplotlib.pyplot as plt

# Đọc dữ liệu từ file
data = np.loadtxt('Data.txt', skiprows=1)

# Lấy tọa độ x, y, z từ dữ liệu
x = data[:, 0]
y = data[:, 1]
z = data[:, 2]

# Tạo subplot 3D
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Vẽ các điểm
ax.scatter(x, y, z)

# Đặt tên cho các trục
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

# Hiển thị đồ thị
plt.show()

from mpl_toolkits.mplot3d.art3d import Poly3DCollection
from mpl_toolkits.mplot3d import Axes3D

# Đọc dữ liệu từ file result.txt
with open('plot.txt') as f:
    lines = f.readlines()

# Tách các giá trị x, y, z của các đỉnh và lưu vào list points
points = []
for line in lines:
    point = [float(p) for p in line.strip().split()]
    points.append(point)

# Tạo danh sách các điểm x, y, z
xs, ys, zs = zip(*points)

# Tách các đỉnh của tam giác và lưu vào list triangles
triangles = []
for i in range(0, len(points), 3):
    triangles.append(points[i:i+3])

# Tạo đối tượng Axes3D
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Vẽ các điểm
ax.scatter(x, y, z)

# Vẽ các tam giác
tri = Poly3DCollection(triangles, facecolors='b', alpha=0.5)
ax.add_collection3d(tri)

# Đặt tên cho các trục
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

# Hiển thị đồ thị
plt.show()
