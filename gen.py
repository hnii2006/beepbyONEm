import sys
import math

path = "b.txt"

def n2s(i):
    s=str(int(i+0.5))
    i = len(s)
    if(i<8):
        s = "00000000"[:(8-i)]+s
    s2 = s[0:len(s)-7]+"."+s[-7:]
    return s2


def wl(n, x1,y1,r):
    if(n==0):
        typ=0
    else:
        typ=3
    st=str(n)+"\t0\t"+str(typ)+"\t16\t0.000000\t0.000000\t"+n2s(r)+"\t0.000000\t"+n2s(y1)+"\t"+n2s(x1)+"\t0.000000\t1\n"
    print(st)
    return st

def gen_path2(args):
    x1 = int(args[1])
    y1 = int(args[2])
    x2 = int(args[3])
    y2 = int(args[4])
    x0 = (x1 + x2) /2
    y0 = (y1 + y2) /2
    xd = x2 - x1
    yd = y2 - y1
    r = int(args[5])
    r = r*1000000
    print("xd,yd,r=",xd,yd,r)
    #from rite to left
    f = open(path,mode='w')
    f.write("QGC WPL 110\n")
    f.write(wl(0,x0,y0,0))
    f.write(wl(1,x1,y1,r))
    f.write(wl(2,x0,y0,0))
    f.write(wl(3,x2,y2,r))
    f.write(wl(4,x0,y0,0))


def gen_path(args):
    x1 = int(args[1])
    y1 = int(args[2])
    x2 = int(args[3])
    y2 = int(args[4])
    x0 = (x1 + x2) /2
    y0 = (y1 + y2) /2
    xd = x2 - x1
    yd = y2 - y1
    r = int(args[5])
#    r = 10
    t = math.atan2(yd,xd)
    rx = math.cos(t)*r
    ry = math.sin(t)*r
    print("rx,ry,t,r=",rx,ry,t,r)
    #from rite to left
    f = open(path,mode='w')
    f.write("QGC WPL 110\n")
    f.write(wl(0,x0,y0,0))
    f.write(wl(1,x1-ry,y1+rx,0))
    f.write(wl(2,x1-rx,y1-ry,0))
    f.write(wl(3,x1+ry,y1-rx,0))
    f.write(wl(4,x0,y0,0))
    f.write(wl(5,x2-ry,y2+rx,0))
    f.write(wl(6,x2+rx,y2+ry,0))
    f.write(wl(7,x2+ry,y2-rx,0))
    f.write(wl(8,x0,y0,0))


    
def check_args(args,num):
    for i in range(num):
        if args[i+1].isdigit()==False:
            print("err:"+args[i+1])
            return False
    return True
	    
if __name__ == '__main__':
    args = sys.argv
    if 5 <= len(args):
        if check_args(args,5):
            gen_path2(args)

        else:
            print('Argument is not digit')
    else:
        print('Arguments are too short')
        print('gen.py 1351234567 351234567 1361234567 351234567 10')
