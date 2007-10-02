#ifndef H_analytics
#define H_analytics

#include "fcontainer.h"
#include "mesh3.h"

class analyticS : public fContainer<double> {
private:
    Vect3 p0,p1,p2; //!< vertices of the triangle
    Vect3 p2p1,p1p0,p0p2;
    Vect3 nu0,nu1,nu2;
    Vect3 n;
    double norme2p2p1,norme2p1p0,norme2p0p2;
    double tanTHETA0m,tanTHETA0p,tanTHETA1m,tanTHETA1p,tanTHETA2m,tanTHETA2p;

public:
    analyticS(){}
    ~analyticS(){}
    void init( int nT, const Mesh &m)
    {
        // all computations needed when the first triangle of integration is changed
        Triangle& T = (Triangle&) m.getTrg(nT);

        p0 = m.getPt(T.s1());
        p1 = m.getPt(T.s2());
        p2 = m.getPt(T.s3());

        p1p0 = p1-p0; p2p1 = p2-p1; p0p2 = p0-p2;
        norme2p1p0 = p1p0.norme(); norme2p2p1 = p2p1.norme(); norme2p0p2 = p0p2.norme();

        n = T.normal();
        n *= -(1/n.norme()) ;

        nu0 = (n^p1p0);
        nu1 = (n^p2p1);
        nu2 = (n^p0p2);
        nu0 *= (1/nu0.norme()) ;
        nu1 *= (1/nu1.norme()) ;
        nu2 *= (1/nu2.norme()) ;
    }

    void init( const Vect3 v0, const Vect3 v1,const Vect3 v2)
    {
        // all computations needed when the first triangle of integration is changed

        p0 = v0;
        p1 = v1;
        p2 = v2;

        p1p0 = p1-p0; p2p1 = p2-p1; p0p2 = p0-p2;
        norme2p1p0 = p1p0.norme(); norme2p2p1 = p2p1.norme(); norme2p0p2 = p0p2.norme();

        n = p1p0^p0p2;
        n *= -(1/n.norme()) ;

        nu0 = (n^p1p0);
        nu1 = (n^p2p1);
        nu2 = (n^p0p2);
        nu0 *= (1/nu0.norme()) ;
        nu1 *= (1/nu1.norme()) ;
        nu2 *= (1/nu2.norme()) ;
    }


    inline double f(const Vect3& x) const
    {
        // analytical value of the internal integral of S operator at point X
        Vect3 p1x = p1-x, p2x = p2-x, p0x = p0-x ;
        double norme2p0x = p0x.norme();
        double norme2p1x = p1x.norme();
        double norme2p2x = p2x.norme();
        double alpha = (x-p0)*n ;
        double g0,g1,g2;

        if ((p0x^p1p0).norme() > .00000001)
            g0 = -log(norme2p1x-p1x*p1p0*(1.0/norme2p1p0) )+log(norme2p0x-p0x*p1p0*(1.0/norme2p1p0) );
        else
            g0= fabs(log(norme2p1x)-log(norme2p0x));
        if ((p1x^p2p1).norme() > .00000001)
            g1 = -log(norme2p2x-p2x*p2p1*(1.0/norme2p2p1) )+log(norme2p1x-p1x*p2p1*(1.0/norme2p2p1) );
        else
            g1= fabs(log(norme2p2x)-log(norme2p1x));
        if ((p2x^p0p2).norme() > .00000001)
            g2 = -log(norme2p0x-p0x*p0p2*(1.0/norme2p0p2) )+log(norme2p2x-p2x*p0p2*(1.0/norme2p0p2) );
        else
            g2 = fabs(log(norme2p0x)-log(norme2p2x));
        return ((p0x*nu0)*g0+(p1x*nu1)*g1+(p2x*nu2)*g2)-alpha*x.solangl(p0,p1,p2);
    }
};

class analyticD : public fContainer<double> {
private:
    Vect3 v1,v2,v3;
    int i;
    double aire;
public:
    analyticD(){}
    ~analyticD(){}
    inline void init( const Mesh& m1, const int trg, const int noeud)
    {
        v1 = m1.getPt(m1.getTrg(trg).s1());
        v2 = m1.getPt(m1.getTrg(trg).s2());
        v3 = m1.getPt(m1.getTrg(trg).s3());
        i = m1.getTrg(trg).contains(noeud);
        aire = m1.getTrg(trg).getArea();
    }

    inline double f(const Vect3& x) const
    {
        //Analytical value of the inner integral in operator D. See DeMunck article for further details.
        //  for non-optimized version of operator D
        //  returns the value of the inner integral of operator D on a triangle used for a P1 function
        Vect3 Y1 = v1-x;
        Vect3 Y2 = v2-x;
        Vect3 Y3 = v3-x;
        double y1 = Y1.norme();
        double y2 = Y2.norme();
        double y3 = Y3.norme();
        double d = Y1*(Y2^Y3);

        double derr = 1e-10;
        if(fabs(d)<derr) return 0.0;

        double omega = 2*atan2(d,(y1*y2*y3+y1*(Y2*Y3)+y2*(Y3*Y1)+y3*(Y1*Y2)));

        Vect3 Z1 = Y2^Y3;
        Vect3 Z2 = Y3^Y1;
        Vect3 Z3 = Y1^Y2;
        Vect3 D1 = Y2-Y1;
        Vect3 D2 = Y3-Y2;
        Vect3 D3 = Y1-Y3;
        double d1 = D1.norme();
        double d2 = D2.norme();
        double d3 = D3.norme();
        double g1 = -1/d1*log((y1*d1+Y1*D1)/(y2*d1+Y2*D1));
        double g2 = -1/d2*log((y2*d2+Y2*D2)/(y3*d2+Y3*D2));
        double g3 = -1/d3*log((y3*d3+Y3*D3)/(y1*d3+Y1*D3));
        Vect3 N = Z1+Z2+Z3;
        double A = N.norme2();
        Vect3 S = D1*g1+D2*g2+D3*g3;
        double omega_i[3];
        omega_i[0] = 1/A*(Z1*N*omega+d*(D2*S));
        omega_i[1] = 1/A*(Z2*N*omega+d*(D3*S));
        omega_i[2] = 1/A*(Z3*N*omega+d*(D1*S));
        double result = omega_i[i-1];

        return result;
    }
};


class analyticD3 : public fContainer<Vect3> {
private:
    Vect3 v1,v2,v3;
    double aire;
public:
    analyticD3(){}
    ~analyticD3(){}
    inline void init( const Mesh& m1, const int trg)
    {
        v1 = m1.getPt(m1.getTrg(trg).s1());
        v2 = m1.getPt(m1.getTrg(trg).s2());
        v3 = m1.getPt(m1.getTrg(trg).s3());
        aire = m1.getTrg(trg).getArea();
    }

    inline Vect3 f(const Vect3& x) const
    {
        //Analytical value of the inner integral in operator D. See DeMunck article for further details.
        //  for non-optimized version of operator D
        //  returns in a vector, the inner integrals of operator D on a triangle viewed as a part of the 3
        //  P1 functions it has a part in.
        Vect3 Y1 = v1-x;
        Vect3 Y2 = v2-x;
        Vect3 Y3 = v3-x;
        double y1 = Y1.norme();
        double y2 = Y2.norme();
        double y3 = Y3.norme();
        double d = Y1*(Y2^Y3);

        double derr = 1e-10;
        if(fabs(d)<derr) return 0.0;

        double omega = 2*atan2(d,(y1*y2*y3+y1*(Y2*Y3)+y2*(Y3*Y1)+y3*(Y1*Y2)));

        Vect3 Z1 = Y2^Y3;
        Vect3 Z2 = Y3^Y1;
        Vect3 Z3 = Y1^Y2;
        Vect3 D1 = Y2-Y1;
        Vect3 D2 = Y3-Y2;
        Vect3 D3 = Y1-Y3;
        double d1 = D1.norme();
        double d2 = D2.norme();
        double d3 = D3.norme();
        double g1 = -1/d1*log((y1*d1+Y1*D1)/(y2*d1+Y2*D1));
        double g2 = -1/d2*log((y2*d2+Y2*D2)/(y3*d2+Y3*D2));
        double g3 = -1/d3*log((y3*d3+Y3*D3)/(y1*d3+Y1*D3));
        Vect3 N = Z1+Z2+Z3;
        double A = N.norme2();
        Vect3 S = D1*g1+D2*g2+D3*g3;
        Vect3 omega_i;
        omega_i.x() = 1/A*(Z1*N*omega+d*(D2*S));
        omega_i.y() = 1/A*(Z2*N*omega+d*(D3*S));
        omega_i.z() = 1/A*(Z3*N*omega+d*(D1*S));

        return omega_i;
    }
};

class analyticDipPot: public fContainer<double> {
private:
    Vect3 q,r0;
public:
    analyticDipPot(){}
    ~analyticDipPot(){}

    inline void init( const Vect3& _q, const Vect3& _r0)
    {
        q = _q;
        r0 = _r0;
    }

    inline double f(const Vect3& x) const
    {
        // RK: A = q.(x-r0)/||^3
        Vect3 r = x-r0;
        double rn = r.norme();
        return (q*r)/(rn*rn*rn);
    }
};

class analyticDipPotDer : public fContainer<Vect3> {
private:
    Vect3 q,r0;
    Vect3 H0,H1,H2;
    Vect3 H0p0DivNorm2,H1p1DivNorm2,H2p2DivNorm2,n;
public:
    analyticDipPotDer(){}
    ~analyticDipPotDer(){}
    inline void init( const Mesh& m, const int nT, const Vect3 &_q, const Vect3 _r0)
    {
        q = _q;
        r0 = _r0;

        Triangle& T = (Triangle&)m.getTrg(nT);

        Vect3 p0,p1,p2,p1p0,p2p1,p0p2,p1p0n,p2p1n,p0p2n,p1H0,p2H1,p0H2;
        p0 = m.getPt(T.s1());
        p1 = m.getPt(T.s2());
        p2 = m.getPt(T.s3());

        p1p0 = p0-p1; p2p1 = p1-p2; p0p2 = p2-p0;
        p1p0n = p1p0; p1p0n.normalize(); p2p1n = p2p1; p2p1n.normalize(); p0p2n = p0p2; p0p2n.normalize();

        p1H0 = (p1p0*p2p1n)*p2p1n; H0 = p1H0+p1; H0p0DivNorm2 = p0-H0; H0p0DivNorm2 = H0p0DivNorm2/H0p0DivNorm2.norme2();
        p2H1 = (p2p1*p0p2n)*p0p2n; H1 = p2H1+p2; H1p1DivNorm2 = p1-H1; H1p1DivNorm2 = H1p1DivNorm2/H1p1DivNorm2.norme2();
        p0H2 = (p0p2*p1p0n)*p1p0n; H2 = p0H2+p0; H2p2DivNorm2 = p2-H2; H2p2DivNorm2 = H2p2DivNorm2/H2p2DivNorm2.norme2();

        n = -p1p0^p0p2;
        n.normalize();
    }

    inline Vect3 f(const Vect3& x) const
    {
        Vect3 P1part(H0p0DivNorm2*(x-H0),H1p1DivNorm2*(x-H1),H2p2DivNorm2*(x-H2));

        // RK: B = n.grad_x(A) with grad_x(A)= q/||^3 - 3r(q.r)/||^5 
        Vect3 r = x-r0;
        double rn = r.norme();
        double EMpart = n*(q/pow(rn,3.)-3*(q*r)*r/pow(rn,5.));

        return -EMpart*P1part; // RK: why - sign ?
    }

};

////////// Gradients wrt r0 (and q)
class analyticDipPotGrad: public fContainer< vect3array<2> > {
private:
    Vect3 q,r0;
public:
    analyticDipPotGrad(){}
    ~analyticDipPotGrad(){}

    inline void init( const Vect3 &_q, const Vect3 &_r0)
    {
        q = _q;
        r0 = _r0;
    }

    inline vect3array<2> f(const Vect3& x) const
    {
        Vect3 r = x-r0;
        double rn = r.norme();
        vect3array<2> res;
        // grad_r0(A)= -q/||^3 + 3 r (q.r)/||^5
        res(0) = (3*(q*r)*r/pow(rn,5.)-q/pow(rn,3.));
        // grad_q(A)= r||^3
        res(1) = r/pow(rn,3.);
        return res;
    }
};

class analyticDipPotDerGrad : public fContainer< vect3array<6> > {
private:
    Vect3 q,r0;
    Vect3 H0,H1,H2;
    Vect3 H0p0DivNorm2,H1p1DivNorm2,H2p2DivNorm2,n;
public:
    analyticDipPotDerGrad(){}
    ~analyticDipPotDerGrad(){}
    inline void init( const Mesh& m, const int nT, const Vect3 &_q, const Vect3 _r0)
    {
        q = _q;
        r0 = _r0;

        Triangle& T = (Triangle&)m.getTrg(nT);

        Vect3 p0,p1,p2,p1p0,p2p1,p0p2,p1p0n,p2p1n,p0p2n,p1H0,p2H1,p0H2;
        p0 = m.getPt(T.s1());
        p1 = m.getPt(T.s2());
        p2 = m.getPt(T.s3());


        p1p0 = p0-p1; p2p1 = p1-p2; p0p2 = p2-p0;
        p1p0n = p1p0; p1p0n.normalize(); p2p1n = p2p1; p2p1n.normalize(); p0p2n = p0p2; p0p2n.normalize();

        p1H0 = (p1p0*p2p1n)*p2p1n; H0 = p1H0+p1; H0p0DivNorm2 = p0-H0; H0p0DivNorm2 = H0p0DivNorm2/H0p0DivNorm2.norme2();
        p2H1 = (p2p1*p0p2n)*p0p2n; H1 = p2H1+p2; H1p1DivNorm2 = p1-H1; H1p1DivNorm2 = H1p1DivNorm2/H1p1DivNorm2.norme2();
        p0H2 = (p0p2*p1p0n)*p1p0n; H2 = p0H2+p0; H2p2DivNorm2 = p2-H2; H2p2DivNorm2 = H2p2DivNorm2/H2p2DivNorm2.norme2();

        n = -p1p0^p0p2;
        n.normalize();

    }

    inline vect3array<6> f(const Vect3& x) const
    {
        Vect3 P1part(H0p0DivNorm2*(x-H0),H1p1DivNorm2*(x-H1),H2p2DivNorm2*(x-H2));

        Vect3 r = x-r0;
        double rn = r.norme();
        // grad_r0(B)= 3/||^5 [ (q.n)r + (q.r)n + (r.n)[q-5(q.r)r/||^2] ]
        Vect3 EMpartR0 = 3/pow(rn,5.)*(
            (q*n)*r
            +(q*r)*n
            +(r*n)*(q-5*(q*r)/(rn*rn)*r)
            );
        // grad_q(B) = n/||^3-3*(n.r)r/||^5
        Vect3 EMpartQ = n/pow(rn,3.)-3*(n*r)/pow(rn,5.)*r;

        vect3array<6> res;
        res(0) = -EMpartR0[0]*P1part; // d/dr0[0]		// RK: why - sign?
        res(1) = -EMpartR0[1]*P1part; // d/dr0[1]
        res(2) = -EMpartR0[2]*P1part; // d/dr0[2]
        res(3) = -EMpartQ[0]*P1part; // d/dq[0]
        res(4) = -EMpartQ[1]*P1part; // d/dq[1]
        res(5) = -EMpartQ[2]*P1part; // d/dq[2]
        return res;

    }
};

#endif