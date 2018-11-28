import math
with open("sample.txt","r") as f:
    coords=f.read().split("\n")
    i=0
    while i<len(coords)-1:
        coo=coords[i].split(",")
        x=int(coo[0])
        y=int(coo[1])
        l1=math.sqrt(math.pow(x,2)+math.pow(600-y,2))*42.8
        l2=math.sqrt(math.pow(460-x,2)+math.pow(600-y,2))*44.16
        i+=1
        with open("sample1.txt","a") as f1:
            f1.write(str(int(l1))+","+str(int(l2))+"\n")
            f1.close()
    
