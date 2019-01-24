# log into Kaggle
# go to "My Account"

# click on Create New API Token
# this will download a file called "kaggle.json"
  # you'll be using this file in colab to access kaggle datasets and competitions
  
# Kaggle API Documentation:
# https://github.com/Kaggle/kaggle-api

## RUN THIS CELL ##
# uploading your kaggle.json file
from google.colab import files
files.upload()


## RUN THIS CELL ##
# installing the kaggle api
!pip install -q kaggle


## RUN THIS CELL ##
# moving the kaggle.json file into a newly created folder called kaggle

# makes the directory
!mkdir -p ~/.kaggle

# moves the file
!cp kaggle.json ~/.kaggle/


## RUN THIS CELL ##
# now you fuckin' have access to the kaggle datasets
!kaggle datasets list


## RUN THIS CELL ##
# do you want to be able to search through the datasets instead of just 
# having a giant list?

!kaggle datasets list -s graduate

# you can only use one search word (afer the -s)
