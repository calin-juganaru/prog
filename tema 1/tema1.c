#include <stdio.h>
#include <math.h>

#define _min(a, b) (a < b ? a : b)
#define _max(a, b) (a > b ? a : b)

#define min(a, b, c) (_min(_min(a, b), c))
#define max(a, b, c) (_max(_max(a, b), c))


int main()
{
    int M, N, cond;
    float R, G, B;
    char c;
    float H, S, V, x;
    float cmax, cmin, delta;

    scanf("%d %d %c %f", &N, &M, &c, &x);
    cond = N * M;

    while(cond--)
    {
        R = G = B = 0.0;
        H = S = V = 0.0;

        scanf("%f %f %f", &R, &G, &B);
        R /= 255; G /= 255; B /= 255;

        cmax = max(R, G, B);
        cmin = min(R, G, B);
        delta = cmax - cmin;

        if(cmax == cmin)
            H = 0;
        else if(cmax == R)
            H = fmodf((G - B) / delta, 6);
        else if(cmax == G)
            H = ((B - R) / delta) + 2;
        else if(cmax == B)
            H = ((R - G) / delta) + 4;

        H *= 60;
        if(H < 0)
            H += 360;
        H /= 360;

        S = (cmax > 0) ? (delta / cmax) : 0;
        V = cmax;

        if(c == 'h')
        {
            if(H + x < 0) H = 0;
            else H = _min(H + x, 1.0);
        }
        else if(c == 's')
            S = _min(S * x, 1.0);
        else if(c == 'v')
            V = _min(V * x, 1.0);

        printf("%.5f %.5f %.5f\n", H, S, V);
    }
    
    return 0;
}
