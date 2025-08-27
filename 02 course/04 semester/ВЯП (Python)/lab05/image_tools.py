from tkinter import filedialog
from PIL import Image, ImageEnhance, ImageFilter, ImageOps

def open_image_file():
  path = filedialog.askopenfilename()
  if path:
    img = Image.open(path)
    return img.copy(), img.copy()
  return None, None

def save_image_file(img):
  if img:
    path = filedialog.asksaveasfilename(
      defaultextension='.jpg',
      filetypes=[('PNG files', '*.png'), ('JPEG files', '*.jpg'), ('All files', '*.*')],
      title='Создать изображение как' 
    )
    if path:
      img.save(path)

def reset_image_state(original):
  return original.copy() if original else None

def rotate_image(img):
  return img.rotate(-90, expand=True) if img else None

def convert_to_grayscale(img):
  return img.convert('L') if img else None

def increase_brightness(img):
  return ImageEnhance.Brightness(img).enhance(1.1) if img else None

def blur_image(img):
  return img.filter(ImageFilter.BLUR) if img else None

def change_contrast(original, factor):
  if original:
    if factor == 1.0:
      return original.copy()
    else:
      img = original.copy()
      return ImageEnhance.Contrast(img).enhance(factor)
  return None

def flip_horizontal(img):
  return img.transpose(Image.FLIP_LEFT_RIGHT) if img else None

def flip_vertical(img):
  return img.transpose(Image.FLIP_TOP_BOTTOM) if img else None

def sharpen_image(img):
  return img.filter(ImageFilter.SHARPEN) if img else None

def edge_detect(img):
  return img.filter(ImageFilter.FIND_EDGES) if img else None

def emboss_image(img):
  return img.filter(ImageFilter.EMBOSS) if img else None

def sepia_image(img):
  if img:
    sepia = img.convert('RGB')
    pixels = sepia.load()
    for y in range(sepia.size[1]):
      for x in range(sepia.size[0]):
        r, g, b = pixels[x, y]
        tr = int(0.393 * r + 0.769 * g + 0.189 * b)
        tg = int(0.349 * r + 0.686 * g + 0.168 * b)
        tb = int(0.272 * r + 0.534 * g + 0.131 * b)
        pixels[x, y] = (min(255, tr), min(255, tg), min(255, tb))
    return sepia
  return None

def invert_colors(img):
  return ImageOps.invert(img.convert('RGB')) if img else None

def posterize_image(img, bits=2):
  return ImageOps.posterize(img.convert('RGB'), bits) if img else None

def solarize_image(img, threshold=128):
  return ImageOps.solarize(img.convert('RGB'), threshold) if img else None