#############################################
# sum.py
# Sample program for testing while statement
#############################################
total = 0
count = 0
while count < 3:
    number=input("Enter a number: ")
    count = count + 1
    total = total + number
    pass
print ("The total is ", total)
