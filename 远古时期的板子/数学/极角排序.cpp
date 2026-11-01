const long double pi = acosl(-1);
long double rad(int x,int y){
    long double t = 0;
    if(x>0 and y>0) t = atan(fabs(y/x));
    else if(x == 0 and y > 0) t = pi/2.;
    else if(x<0 and y>0) t = pi-atan(fabs(y/x));
    else if(x<0 and y == 0) t = pi;
    else if(x<0 and y<0) t = pi+atan(fabs(y/x));
    else if(x == 0 and y<0) t = pi*3/2;
    else if(x >0 and y<0) t = 2*pi-atan(fabs(y/x));
    else t = 2*pi;
    return t;
}

struct node{
    int x,y;
    node(int xx = 0,int yy = 0):x(xx),y(yy){}
    bool isUpRound()const{return y > 0 || (x>0 and y == 0);}
    bool operator==(node& t) {return t.x == x and t.y == y;}
};
struct Cmp{
    bool operator()(const node &a,const node &b)const{
        int ta = a.isUpRound(),tb = b.isUpRound();
        if(ta != tb) return ta<tb;
        else return a.y*b.x<a.x*b.y; 
    }
};
bool cmp(const node &a,const node &b){ // return a<b;
    int ta = a.isUpRound(),tb = b.isUpRound();
    if(ta != tb) return ta<tb;
    else return a.y*b.x<a.x*b.y; 
}
// 上述均基于从x正半轴随着逆时针角度递增到 2*pi,x正半轴为0
// { x/g , y/g }存方向
