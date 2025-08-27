class Car:
  def __init__(self, speed, engine_volume):
    self.speed = speed
    self.engine_volume = engine_volume

  @property
  def speed(self):
    return self._speed

  @speed.setter
  def speed(self, value):
    if not isinstance(value, (int, float)) or value < 0:
      raise ValueError('Скорость должна быть положительной')
    self._speed = value

  @property
  def engine_volume(self):
    return self._engine_volume

  @engine_volume.setter
  def engine_volume(self, value):
    if not isinstance(value, (int, float)) or value <= 0:
      raise ValueError('Объем двигателя должен быть положительным')
    self._engine_volume = value

  def move(self):
    print('Еду на машине')

  def __str__(self):
    return f'Машина: Скорость {self.speed} км/ч, Объем двигателя {self.engine_volume} л'

  def __lt__(self, other):
    if self.speed < other.speed:
      print(f'{self.model} имеет скорость больше, чем {other.model}')
    else:
      print(f'У второй машины скорость больше')


class Honda(Car):
  def __init__(self, model, speed, engine_volume):
    super().__init__(speed, engine_volume)
    self.model = model

  @property
  def model(self):
    return self._model

  @model.setter
  def model(self, value):
    if not isinstance(value, str) or not value:
      raise ValueError('Модель должна быть настоящей строкой')
    self._model = value

  def move(self):
    super().move()
    print('Еду на Honda')

  def __str__(self):
    return super().__str__() + f', Модель: {self.model}'


class Volvo(Car):
  def __init__(self, model, speed, engine_volume):
    super().__init__(speed, engine_volume)
    self.model = model

  @property
  def model(self):
    return self._model

  @model.setter
  def model(self, value):
    if not isinstance(value, str) or not value:
      raise ValueError('Модель должна быть настоящей строкой')
    self._model = value

  def move(self):
    super().move()
    print('Еду на Volvo')

  def __str__(self):
    return super().__str__() + f', Модель: {self.model}'


class BMW(Car):
  def __init__(self, model, speed, engine_volume):
    super().__init__(speed, engine_volume)
    self.model = model

  @property
  def model(self):
    return self._model

  @model.setter
  def model(self, value):
    if not isinstance(value, str) or not value:
      raise ValueError('Модель должна быть настоящей строкой')
    self._model = value

  def move(self):
    super().move()
    print('Еду на BMW')

  def __str__(self):
    return super().__str__() + f', Модель: {self.model}'


cars = [
  Honda('Civic', 180, 1.8),
  Volvo('15DR', 200, 2.5),
  BMW('X5', 220, 3.0)
]

print('-' * 30)

for car in cars:
  print(car)

print('-' * 30)

for car in cars:
  car.move()
  print('-' * 20)

max_engine_car = max(cars, key=lambda c: c.engine_volume)
print(f'Машина с наибольшим объемом двигателя: {max_engine_car}')

cars[0] < cars[1]
