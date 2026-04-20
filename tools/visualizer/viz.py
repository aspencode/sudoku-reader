from pathlib import Path

import cv2
import matplotlib.pyplot as plt
from skimage.feature import hog

current_dir = Path(__file__).parent
data_dir = current_dir.parents[1] / "data"
image_path = data_dir / "English" / "Fnt" / "Sample009" / "img009-00122.png"

img = cv2.imread(str(image_path))

gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
resized = cv2.resize(gray, (32, 32))
_, binary = cv2.threshold(resized, 0, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)

desc, hog_image = hog(
    binary,
    orientations=9,
    pixels_per_cell=(4, 4),
    cells_per_block=(2, 2),
    visualize=True,
)

plt.figure(figsize=(8, 4))
plt.subplot(1, 2, 1)
plt.title("Obraz wejściowy")
plt.imshow(binary, cmap="gray")

plt.subplot(1, 2, 2)
plt.title("Cechy HOG")
plt.imshow(hog_image, cmap="gray")

plt.show()
