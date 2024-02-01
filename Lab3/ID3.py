#If running in jupyter notebook you need to be able to use your virtual environment.
#First:
#pip install ipython
#ipython kernel install --user --name=aiinthewild
#This allows you use the virtual environment in jupyter notebook by selecting Kernel->Change Kernel

# Load libraries
import pandas as pd
from sklearn.tree import DecisionTreeClassifier # Import Decision Tree Classifier
from sklearn.model_selection import train_test_split # Import train_test_split function
from sklearn import metrics #Import scikit-learn metrics module for accuracy calculation

col_names = ['pregnant', 'glucose', 'bp', 'skin', 'insulin', 'bmi', 'pedigree', 'age', 'label']
# load dataset
pima = pd.read_csv("diabetes.csv", header=None, names=col_names)

print(pima.head())

#split dataset in features and target variable
feature_cols = ['pregnant', 'insulin', 'bmi', 'age','glucose','bp','pedigree']
X = pima[feature_cols] # Features
y = pima.label # Target variable

# Split dataset into training set and test set
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=1) # 70% training and 30% test
#print(X_train)
#print(X_test)

# Create Decision Tree classifer object using information gain as the splitting criteria.
clf = DecisionTreeClassifier(criterion='entropy')

#################################
#           P1                  #
# Train Decision Tree Classifer
clf = clf.fit(X_train,y_train)

new_patient_data = {}
for feature in feature_cols:
    value = float(input(f"Enter the value (number) for feature '{feature}': "))
    new_patient_data[feature] = [value]

# Create a DataFrame with the user's input data
new_patient_df = pd.DataFrame(new_patient_data)

# Predict whether the new patient has diabetes or not
prediction = clf.predict(new_patient_df)

if prediction[0] == 0:
    print("The model predicts that the new patient does not have diabetes.")
else:
    print("The model predicts that the new patient has diabetes.")

#################################

#Predict the response for test dataset
#Lets print out the first few rows of the test data.
print("Testing:")
print(X_test.head())
y_pred = clf.predict(X_test)

# Model Accuracy, how often is the classifier correct?
print("Accuracy:",metrics.accuracy_score(y_test, y_pred))

#If using a jupyter notebook
#from sklearn.tree import export_graphviz
#import graphviz

#data = export_graphviz(clf, out_file=None,  
#                filled=True, rounded=True,
#                special_characters=True,
#                max_depth=3, 
#                feature_names = feature_cols,class_names=['0','1'])
#graph = graphviz.Source(data, format='png')
#graph

#Otherwise in normal python you can use matplotlib (you will need to pip install it):
import matplotlib.pyplot as plt
from sklearn import tree

plt.figure(figsize=(15,8))
tree.plot_tree(clf, filled=True, rounded=True, max_depth=3, fontsize=10, feature_names=feature_cols,class_names=['0','1'])
plt.title("Decision tree trained on all features")
plt.show()