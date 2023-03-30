import cv2
import numpy as np

path = input("Path to png: ")

img = cv2.imread(path)
res = np.zeros(img.shape[:2])

for y in range(img.shape[0]):
    for x in range(img.shape[1]):
        b, g, r = img[y, x]
        res[y, x] = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)
        


output_file = input("Enter output file name: ")
matrix_name = input("Enter matrix name: ")
#np.savetxt(output_file, res, fmt="%d", delimiter=",")

with open(output_file, "w") as fp:
    fp.write(f"uint16_t {matrix_name}[{img.shape[0]}][{img.shape[1]}] = " + "{\n")
    for y in range(img.shape[0]):
        fp.write("  {")
        for x in range(img.shape[1]):
            fp.write(f"{res[y,x]}")
            if x < img.shape[1] - 1:
                fp.write(",")
        fp.write("}")
        if y < img.shape[0] - 1:
            fp.write(",")
        fp.write("\n")
    fp.write("};\n")
    
print(f"Matrix {matrix_name} written to file: {output_file}")
