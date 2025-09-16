vector<int> prim,phi,minp;
void Phi(int n){
    minp.resize(n+10),phi.resize(n+10);phi[1] = 1;
    for(int i = 2;i<=n;i++){
        if(!minp[i]){
            prim.push_back(i);
            minp[i] = i;
            phi[i] = i-1;
        }
        for(int& p:prim){
            if(i*p>n) break;
            minp[i*p] = p;
            if(minp[i] == p){
                phi[i*p] = phi[i]*p;
                break;
            }else{
                phi[i*p] = phi[i]*(p-1);
            }

        }
    }
}
