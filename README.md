# IPT-2020-FE91mp
Repo for lab works

## Student Info

| Var  | Value |
|:------------- | -------------:|
| Name | Vladislav Sokolovsky |
| Group | FE91mp |
| Course | 5 |
| Variant | 14|
| Report #1 | [Link](https://docs.google.com/document/d/17XYFA0iRnTYShxJMO5zVkqcA0Pe6QfQEyWQBzSSRirc/edit?usp=sharing)|
| Report #2 | [Link](https://docs.google.com/document/d/1M1SfdVymhskQUDKvCqbayXEhfLb1PitCiyb6rvOCaTk/edit?usp=sharing)|

## Вихідні дані

**Тестовий пакет** – [MIRFlickr-20k](https://press.liacs.nl/mirflickr/#sec_download)

**Вибірка зображень** – 250 зображень;

**Формування вибірки зображень** – псевдовипадкове, з використанням генератора Мерсена (стартове значення співпадає з номером студента в загальному списку групи) за модулем кількості зображень в тестовому пакеті.


## Task descriptions

### Лабораторна робота №1
1. Сформувати тестову вибірку зображень з вихідного пакета;
2. Для кожного каналу кольору кожного зображення з тестового пакета обчислити наступні характеристики:
    - Максимальна / мінімальне значення; 
    - Математичне сподівання і дисперсію;
    - Медіану значень, інтерквартільний розмах;
    - Коефіцієнти асиметрії та ексцесу (нормалізований);
3.	Для кожного каналу кольору кожного зображення з тестового пакета побудувати гістограму значень яскравості пікселів;
4.	 Провести апроксимацію отриманих гістограм з використанням відомих імовірнісних розподілів, визначити найкращу апроксимацію;
5.	 Побудувати розподіл типів використаних імовірнісних розподілів для яких досягається мінімальне значення помилки апроксимації з п.4.

### Лабораторна робота №2
1.	Сформувати тестову вибірку зображень з вихідного пакета;
2.	Для кожного каналу кольору кожного зображення з тестового пакета обчислити наступні характеристики:
    - Математичне сподівання і дисперсію;
    - Коефіцієнти асиметрії та ексцесу (нормалізований);
3.	Побудувати вектори параметрів зображень, що складаються з:
    - Математичних очікувань значень яскравості для кожного каналу кольору;
    - Математичних очікувань і дисперсії значень яскравості для кожного каналу кольору;
    - Математичних очікувань, дисперсії і коефіцієнта асиметрії значень яскравості для кожного каналу кольору;
    - Математичних очікувань, дисперсії, коефіцієнтів асиметрії та ексцесу значень яскравості для кожного каналу кольору;
4.	Побудувати гаусові моделі зображень з використанням розрахованих раніше параметрів.
5.	Провести декомпозицію кожного каналу кольору кожного зображення з застосуванням методу головних компонент (PCA):
    - Варіюючи кількість компонент, провести реконструкцію окремих каналів кольору зображень (від компонент з найбільшою енергією поступово переходячи до компонентів з мінімальною енергією).
    - Побудувати залежність помилки відновлення (середнє відхилення вихідного зображення відреконструйованого, MSE) від кількості використаних компонент.
6.	Провести моделювання окремих каналів кольору зображень з використанням марковських ланцюгів:
    - Для кожного каналу кольору кожного зображення розрахувати стохастическую матрицю марковської ланцюга першого і другого порядків (обробка пікселів по горизонталі справа наліво і навпаки, а також по вертикалі зверху вниз і навпаки). У звіті привести явний вигляд однієї марковської ланцюга для одного з каналів кольору тестового зображення;
    - Перевірити властивість регулярності, реккурентное і незворотності (irreducible) для отриманих марковских моделей для 5 ітерацій.


### Лабораторная работа №3:
1. Сформировать тестовую выборку изображений из исходного пакета;
2. Для каждого канала цвета каждого изображения из тестового пакета вычислить следующие характеристики:
    -   Математическое ожидание и дисперсию;
    -   Коэффициенты асимметрии и эксцесса (нормализованный);
3.	Используя модели SPAM и CC-PEV (http://dde.binghamton.edu/download/feature_extractors/), рассчитать вектора характеристик каждого канала цвета каждого изображения;
4.	Полученные параметры изображений упаковать в матрицу признаков (каждая строка соответствует результатам для отдельного канала цвета тестового изображения, каждый столбец - параметру изображения). Матрицы признаков построить отдельно для статистических характеристик изображений (1-4 центральные моменты), а также моделей SPAM и CC-PEV;
5.  Построить вектор меток классов изображений.Название целевого класса для каждого студента определяется согласно позиции студента в списке группы (см. вложенный файл - например, для первого студента в списке группы целевой класс "explore", для второго студента - метка "sky", для третьего студента - метка "nikon" и т.д.);
6.	Провести Псевдослучайным образом разделить исходный пакет изображений на 2 равные части (тестовая и контрольная подвыборки). С использованием тестовой подвыборки провести настройку следующих классификаторов:
    -   Линейная регрессия;
    -   Робастная регрессия;
    -   Логистическая регрессия;
    -   Метод опорных векторов (SVM);
7. Используя настроенные классификаторы из п. 6 провести обработки изображений из контрольного подпакета. Оценить вероятности правильной классификации (0 и 1 классы), а также вероятности ошибок первого (ложное срабатывание) и второго (пропуск цели) рода;
8. Повторить пп. 6-7 10 раз для получения усредненной точности классификации изображений.

