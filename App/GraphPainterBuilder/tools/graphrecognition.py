import cv2
import numpy as np
from scipy.interpolate import interp1d

# 1. ЗАГРУЗКА ЦВЕТНОГО ИЗОБРАЖЕНИЯ
image_path = 'graph.png'  # Имя вашего файла
img = cv2.imread(image_path)
if img is None:
    print(f"Ошибка: Не удалось открыть файл {image_path}")
    exit()

hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

# 2. ВЫДЕЛЕНИЕ СУЩЕСТВУЮЩИХ ЧАСТЕЙ ГРАФИКА
lower_color = np.array([0, 40, 40])     
upper_color = np.array([180, 255, 255]) 
mask = cv2.inRange(hsv, lower_color, upper_color)

# Очистка от мелкого мусора
kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
clean_mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)

# Находим координаты всех существующих пикселей графика
points = np.column_stack(np.where(clean_mask > 0))

if len(points) == 0:
    print("График не найден. Проверьте настройки маски цвета.")
else:
    y_pixels, x_pixels = points[:, 0], points[:, 1]

    # Усредняем Y для каждого существующего X
    unique_x = np.unique(x_pixels)
    unique_y = np.array([np.mean(y_pixels[x_pixels == x]) for x in unique_x])

    # 3. ПОИСК И ЗАПОЛНЕНИЕ РАЗРЫВОВ (ИНТЕРПОЛЯЦИЯ)
    # Создаем ПОЛНУЮ сетку X от самого начала до самого конца графика без пропусков
    x_min, x_max = unique_x.min(), unique_x.max()
    full_x = np.arange(x_min, x_max + 1)

    # Строим математическую модель по ИМЕЮЩИМСЯ точкам
    interp_func = interp1d(unique_x, unique_y, kind='linear', fill_value="extrapolate")
    
    # Вычисляем Y абсолютно для всех X (включая пропущенные зоны)
    full_y = interp_func(full_x)

    # 4. ОТДЕЛЬНАЯ ОТРИСОВКА ТОЛЬКО ДОСТРОЕННЫХ КУСКОВ
    result_img = img.copy()
    
    # Цвет для восстановленных участков (Ярко-красный в формате BGR)
    repair_color = (0, 0, 255) 
    
    # Превращаем массив существующих X в set для мгновенного поиска
    existing_x_set = set(unique_x)

    # Идем по всей длине графика и красим только те пиксели, которых РАНЬШЕ НЕ БЫЛО
    for x, y in zip(full_x, full_y):
        if x not in existing_x_set:
            # Рисуем жирную точку в месте, где был разрыв
            cv2.circle(result_img, (int(x), int(y)), radius=2, color=repair_color, thickness=-1)

    # 5. СОХРАНЕНИЕ
    output_path = 'graph_repaired.png'
    cv2.imwrite(output_path, result_img)
    print(f"Готово! Восстановленные участки выделены цветом. Файл: {output_path}")
