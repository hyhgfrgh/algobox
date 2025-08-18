{
    // 一维部分省略 ...
}

{
    // 二维前缀和  示例 Smilo and Minecraft https://codeforces.com/contest/2113/problem/C
    // 求矩形区域目标点最少 示例
    int n,m,k;cin>>n>>m>>k;
    vector<string> a(n+1);
    for(int i = 1;i<=n;i++){
        cin>>a[i];a[i] = " "+a[i];
    }
    vector<vector<int>> pre(n+1,vector<int> (m+1));
    int sum = 0;
    for(int i = 1;i<=n;i++){
        for(int j = 1;j<=m;j++){
            if(a[i][j] == 'g') pre[i][j]++,sum++;
            pre[i][j] += pre[i][j-1];
        }
        for(int j = 1;j<=m;j++){
            pre[i][j] += pre[i-1][j];
        }
    }
    int mn = 1e9;
    for(int i = 1;i<=n;i++){
        for(int j = 1;j<=m;j++){
            if(a[i][j] == '.'){
                int x1 = min(n,i+k),x2 = max(0LL,i-k-1);
                int y1 = min(m,j+k),y2 = max(0LL,j-k-1);
                int tt = pre[x1][y1]-pre[x1][y2]-pre[x2][y1]+pre[x2][y2];
                mn = min(mn,tt);
            }
        }
    }
}

{
    // 二维差分  示例
    int n,m;cin>>n>>m;
    vector a(n+2,vector<int> (m+2));
    int q;cin>>q;
    while(q--){
        int x1,y1,x2,y2;
        cin>>x1>>y1>>x2>>y2;
        a[x1][y1]++;
        a[x1][y2+1]--;
        a[x2+1][y1]--;
        a[x2+1][y2+1]++;
    }
    for(int i = 1;i<=n;i++){
        for(int j = 1;j<=m;j++){
            a[i][j] += a[i][j-1];
        }
        for(int j = 1;j<=m;j++){
            a[i][j] += a[i-1][j];
            cout<<a[i][j]<<" ";
        }
        cout<<"\n";
    }

}

{
    // 三维前缀和  示例
    int a,b,c;cin>>a>>b>>c;
    vector pre(a+1,vector (b+1,vector<int> (c+1)));
    for(int i = 1;i<=a;i++){
        for(int j = 1;j<=b;j++){
            for(int k = 1;k<=c;k++){
                cin>>pre[i][j][k];
            }
        }
    }
    for(int k = 1;k<=a;k++){
        for(int j = 1;j<=b;j++){
            for(int i = 1;i<=c;i++)
                pre[k][j][i] += pre[k][j][i-1];
            for(int i = 1;i<=c;i++)
                pre[k][j][i] += pre[k][j-1][i];
        }
        for(int j = 1;j<=b;j++){
            for(int i = 1;i<=c;i++)
                pre[k][j][i] += pre[k-1][j][i];
        }
    }
    cout<<pre[a][b][c]<<"\n";
}
{
    ......
}
