from pandas import *
import numpy as np
from sklearn.linear_model import LinearRegression, LogisticRegression
from sklearn.svm import SVR
from sklearn.discriminant_analysis import QuadraticDiscriminantAnalysis as QDA

count_logistic1 = 0
count_logistic2 = 0
count_QDA1 = 0
count_QDA2 = 0
count_svm1 = 0
count_svm2 = 0

set_option('display.max_columns', 50)
set_option('display.width', 500)

data_tune = read_csv('Spam_tune.csv')      #Набор тестовых данных
data_test = read_csv('Spam_test.csv')   # Набор контрольных данных

x = data_tune.iloc[:,:-1]   # Набор признаков
y = data_tune.iloc[:,-1]   # Метки классов

x_train = x
x_test = data_test.iloc[:,:-1]

y_train = y
y_test = data_test.iloc[:,-1]

d = DataFrame(index=y_test.index) # Набор данных для проверки модели
d["Actual"] = y_test                 # Реальные значения тегов

model1 = LogisticRegression()
model1.fit(x_train, y_train)

d['Predict_LR'] = model1.predict(x_test) # Тест
d["Correct_LR"] = (d['Predict_LR']*d["Actual"])   # Считаем процент совпадений

hit_rate1 = np.mean(d["Correct_LR"])
print("Процент верных предсказаний логистической регрессии: %.1f%%" % (hit_rate1*100))

d["Error_LR"] = (d["Actual"]-d['Predict_LR'])
for i in d["Error_LR"]:
	if i == -1:
		count_logistic2 += 1
	elif i == 1:
		count_logistic1 += 1
print("Ошибки 1 рода - %.1f%%" % (count_logistic1/len(d["Error_LR"])*100))
print("Ошибки 2 рода - %.1f%%" % (count_logistic2/len(d["Error_LR"])*100))
print()


# ----------------------- LinearRegression---------------------------
model2 = LinearRegression()
model2.fit(x_train, y_train)

d['Predict_ LinearRegression'] = model2.predict(x_test) # Тест
d["Correct_ LinearRegression"] = (d['Predict_LDA']*d["Actual"])   # Считаем процент совпадений

hit_rate2 = np.mean(d["Correct_ LinearRegression"])
print("Процент верных предсказаний для LinearRegression: %.1f%%" % (hit_rate2*100))

d["Error_ LinearRegression"] = ((d["Actual"])-d['Predict_ LinearRegression'])
for i in d["Error_ LinearRegression"]:
	if i == -1:
		count_svm2 += 1
	elif i == 1:
		count_svm1 += 1

print("Ошибки 1 рода - %.1f%%" % (count_svm1/len(d["Error_ LinearRegression"])*100))
print("Ошибки 2 рода - %.1f%%" % (count_svm2/len(d["Error_ LinearRegression"])*100))
print(d)


model3 = SVM()
model3.fit(x_train, y_train)

count_svm1 = 0
count_svm2 = 0

d['Predict_SVM'] = model3.predict(x_test) # Тест
d["Correct_SVM"] = (d['Predict_SVM']*d["Actual"])   # Считаем процент совпадений

hit_rate3 = np.mean(d["Correct_SVM"])
print("Процент верных предсказаний для SVM: %.1f%%" % (hit_rate3*100))

d["Error_SVM"] = (d["Actual"]-d['Predict_SVM'])
for i in d["Error_SVM"]:
	if i == -1:
		count_svm2 += 1
	elif i == 1:
		count_svm1 += 1

print("Ошибки 1 рода - %.1f%%" % (count_svm1/len(d["Error_SVM"])*100))
print("Ошибки 2 рода - %.1f%%" % (count_svm2/len(d["Error_SVM"])*100))
