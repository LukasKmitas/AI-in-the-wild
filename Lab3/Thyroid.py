#If running in jupyter notebook you need to be able to use your virtual environment.
#First:
#pip install ipython
#ipython kernel install --user --name=aiinthewild
#This allows you use the virtual environment in jupyter notebook by selecting Kernel->Change Kernel

# Load libraries
from matplotlib import pyplot as plt
import pandas as pd
from sklearn import tree
from sklearn.tree import DecisionTreeClassifier # Import Decision Tree Classifier
from sklearn.model_selection import train_test_split # Import train_test_split function
from sklearn import metrics #Import scikit-learn metrics module for accuracy calculation

col_names = ['Age', 'Gender', 'Smoking', 'Hx Smoking', 'Hx Radiothreapy',
              'Thyroid Function', 'Physical Examination', 'Adenopathy', 'Pathology', "Focality",
                "Risk", "T", "N", "M", "Stage", "Response", "Recurred"]
# load dataset
pima = pd.read_csv("Thyroid_Diff.csv", header=None, names=col_names)

# Replace binary categorical variables with 0 and 1
pima['Gender'] = pima['Gender'].replace({'F': 1, 'M': 0})
pima['Smoking'] = pima['Smoking'].replace({'Yes': 1, 'No': 0})
pima['Hx Smoking'] = pima['Hx Smoking'].replace({'Yes': 1, 'No': 0})
pima['Hx Radiotherapy'] = pima['Hx Radiotherapy'].replace({'Yes': 1, 'No': 0})
pima['Recurred'] = pima['Recurred'].replace({'Yes': 1, 'No': 0})

print(pima.head())

#split dataset in features and target variable
feature_cols = ['Age', 'Gender', 'Smoking', 'Hx Smoking','Hx Radiothreapy','Adenopathy','Recurred'
                'Risk', 'Stage', 'Recurred']
X = pima[feature_cols] # Features
y = pima.label # Target variable

# Split dataset into training set and test set
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=1) # 70% training and 30% test
#print(X_train)
#print(X_test)

# Create Decision Tree classifer object using information gain as the splitting criteria.
clf = DecisionTreeClassifier(criterion='entropy')

# Train Decision Tree Classifer
y = pima['Recurred']  # Target variable

# Split dataset into training set and test set
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=1)

# Create Decision Tree classifier object
clf = DecisionTreeClassifier(criterion='entropy')

# Train Decision Tree Classifier
clf = clf.fit(X_train, y_train)

# Predict the response for test dataset
y_pred = clf.predict(X_test)

# Model Accuracy
print("Accuracy:", metrics.accuracy_score(y_test, y_pred))

# Plot the tree
plt.figure(figsize=(15,8))
tree.plot_tree(clf, filled=True, rounded=True, max_depth=3, fontsize=10, feature_names=feature_cols, class_names=['No', 'Yes'])
plt.title("Decision tree trained on thyroid dataset")
plt.show()