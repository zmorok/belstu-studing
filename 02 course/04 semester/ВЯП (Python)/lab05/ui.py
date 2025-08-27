import tkinter as tk
from PIL import ImageTk

from image_tools import *
from state import ImageState

class ImageEditorApp:
  def __init__(self, root):
    self.root = root
    self.root.title('Редактор изображений')
    self.root.geometry('1000x700')
    self.root.resizable(False, False)

    self.state = ImageState()
    self.orig_img = self.state.image

    self.root.grid_columnconfigure(0, weight=3)
    self.root.grid_columnconfigure(1, weight=1)
    self.root.grid_rowconfigure(1, weight=3)

    self.build_top_panel()
    self.build_canvas()
    self.build_side_panel()

  def build_top_panel(self):
    top_frame = tk.Frame(self.root)
    top_frame.grid(row=0, column=0, columnspan=3, pady=20)

    tk.Button(top_frame, text='Открыть', command=self.open_image, width=25, height=3).grid(row=0, column=0, padx=15)
    tk.Button(top_frame, text='Сбросить\nфильтры', command=self.reset_image, width=25, height=3).grid(row=0, column=1, padx=15)
    tk.Button(top_frame, text='Сохранить', command=self.save_image, width=25, height=3).grid(row=0, column=2, padx=15)
    tk.Button(top_frame, text='Очистить\nполотно', command=self.delete_image, width=25, height=3).grid(row=0, column=3, padx=15)

  def build_canvas(self):
    self.canvas = tk.Canvas(self.root, bg='lightgray')
    self.canvas.grid(row=1, column=0, padx=20, pady=20, sticky='nsew')

  def build_side_panel(self):
    side_wrapper = tk.Frame(self.root)
    side_wrapper.grid(row=1, column=1, sticky='ns')

    side_frame = tk.LabelFrame(side_wrapper, text='Редактирование', padx=10, pady=10)
    side_frame.grid(row=1, column=0, padx=10, pady=10)

    tk.Button(side_frame, text='Повернуть', command=self.rotate_image).grid(row=0, column=0, sticky='ew', pady=2)
    tk.Button(side_frame, text='Ч/Б', command=self.to_gray).grid(row=1, column=0, sticky='ew', pady=2)
    tk.Button(side_frame, text='Ярко', command=self.brighten).grid(row=2, column=0, sticky='ew', pady=2)
    tk.Button(side_frame, text='Блюр', command=self.blur).grid(row=3, column=0, sticky='ew', pady=2)
    tk.Button(side_frame, text='Отразить по X', command=self.flip_horizontal).grid(row=5, column=0, sticky='ew', pady=2)
    tk.Button(side_frame, text='Отразить по Y', command=self.flip_vertical).grid(row=6, column=0, sticky='ew', pady=2)
    tk.Button(side_frame, text='Резкость', command=self.sharpen).grid(row=7, column=0, sticky='ew', pady=2)
    tk.Button(side_frame, text='Контур', command=self.edge_detect).grid(row=8, column=0, sticky='ew', pady=2)
    tk.Button(side_frame, text='Эмбосс', command=self.emboss).grid(row=9, column=0, sticky='ew', pady=2)
    tk.Button(side_frame, text='Сепия', command=self.sepia).grid(row=10, column=0, sticky='ew', pady=2)
    tk.Button(side_frame, text='Инверсия', command=self.invert).grid(row=11, column=0, sticky='ew', pady=2)
    tk.Button(side_frame, text='Постеризация', command=self.posterize).grid(row=12, column=0, sticky='ew', pady=2)
    tk.Button(side_frame, text='Соляризация', command=self.solarize).grid(row=13, column=0, sticky='ew', pady=2)


    self.contrast_slider = tk.Scale(side_frame, from_=0.5, to=2.0, resolution=0.1,
                                        orient='horizontal', label='Контраст', command=self.change_contrast)
    self.contrast_slider.set(1.0)
    self.contrast_slider.grid(row=4, column=0, sticky='ew', pady=10)

  def show_image(self):
    if self.state.image:
      self.canvas.delete('all')
      canvas_width = self.canvas.winfo_width() or 700
      canvas_height = self.canvas.winfo_height() or 500

      img = self.state.image
      width, height = img.size
      scale = min(canvas_width / width, canvas_height / height)
      new_size = (int(width * scale), int(height * scale))

      resized_img = img.resize(new_size)
      self.tk_img = ImageTk.PhotoImage(resized_img)

      x = (canvas_width - new_size[0]) // 2
      y = (canvas_height - new_size[1]) // 2
      self.canvas.create_image(x, y, anchor='nw', image=self.tk_img)

  def open_image(self):
    self.state.image, self.state.original = open_image_file()
    self.show_image()

  def save_image(self):
    save_image_file(self.state.image)

  def reset_image(self):
    self.state.image = reset_image_state(self.state.original)
    self.contrast_slider.set(1.0)
    self.show_image()

  def delete_image(self):
    self.state.clear()
    self.canvas.delete('all')
    self.contrast_slider.set(1.0)

  def rotate_image(self):
    self.state.image = rotate_image(self.state.image)
    self.show_image()

  def to_gray(self):
    self.state.image = convert_to_grayscale(self.state.image)
    self.show_image()

  def brighten(self):
    self.state.image = increase_brightness(self.state.image)
    self.show_image()

  def blur(self):
    self.state.image = blur_image(self.state.image)
    self.show_image()

  def change_contrast(self, val):
    self.state.image = change_contrast(self.state.original, float(val))
    self.show_image()
    
  def flip_horizontal(self):
    self.state.image = flip_horizontal(self.state.image)
    self.show_image()

  def flip_vertical(self):
    self.state.image = flip_vertical(self.state.image)
    self.show_image()

  def sharpen(self):
    self.state.image = sharpen_image(self.state.image)
    self.show_image()

  def edge_detect(self):
    self.state.image = edge_detect(self.state.image)
    self.show_image()

  def emboss(self):
    self.state.image = emboss_image(self.state.image)
    self.show_image()

  def sepia(self):
    self.state.image = sepia_image(self.state.image)
    self.show_image()

  def invert(self):
    self.state.image = invert_colors(self.state.image)
    self.show_image()

  def posterize(self):
    self.state.image = posterize_image(self.state.image, bits=2)
    self.show_image()

  def solarize(self):
    self.state.image = solarize_image(self.state.image, threshold=128)
    self.show_image()

