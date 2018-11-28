x=230
y=300
with open('sample.txt','w') as f:
    while y<=400:
          f.write(str(x)+","+str(y)+"\n")
          y+=1
    while x<=330:
          f.write(str(x)+","+str(y)+"\n")
          x+=1
    while y>=300:
          f.write(str(x)+","+str(y)+"\n")
          y-=1
    while x>=230:
          f.write(str(x)+","+str(y)+"\n")
          x-=1
    f.close()