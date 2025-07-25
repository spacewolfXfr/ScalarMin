#!/usr/bin/python3

# Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
# 
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

import numpy as np
import typing

import matplotlib.pyplot as plt

#################### Test function ####################

def test_fun(Dx:float, Dy:float, r1:float, w1:float, r2:float, w2:float, Dphi:float, t:float) -> float:
    assert r1 >= 0
    assert r2 >= 0
    
    x = Dx + r1*np.cos(w1*t) - r2*np.cos(w2*t+Dphi)
    y = Dy + r1*np.sin(w1*t) - r2*np.sin(w2*t+Dphi)
    return x*x + y*y

def test_fun_d(Dx:float, Dy:float, r1:float, w1:float, r2:float, w2:float, Dphi:float, t:float) -> float:
    assert r1 >= 0
    assert r2 >= 0
    
    c1 = np.cos(w1*t)
    s1 = np.sin(w1*t)

    c2 = np.cos(w2*t+Dphi)
    s2 = np.sin(w2*t+Dphi)

    x = Dx + r1*c1 - r2*c2
    y = Dy + r1*s1 - r2*s2

    x_d = -w1*r1*s1 + w2*r2*s2
    y_d =  w1*r1*c1 - w2*r2*c2

    return (x_d*x + y_d*y)*2

def test_fun_dd(Dx:float, Dy:float, r1:float, w1:float, r2:float, w2:float, Dphi:float, t:float) -> float:
    assert r1 >= 0
    assert r2 >= 0
    
    c1 = np.cos(w1*t)
    s1 = np.sin(w1*t)

    c2 = np.cos(w2*t+Dphi)
    s2 = np.sin(w2*t+Dphi)

    x = Dx + r1*c1 - r2*c2
    y = Dy + r1*s1 - r2*s2

    x_d = -w1*r1*s1 + w2*r2*s2
    y_d =  w1*r1*c1 - w2*r2*c2

    x_dd = -w1*w1*r1*c1 + w2*w2*r2*c2
    y_dd = -w1*w1*r1*s1 + w2*w2*r2*s2

    return (x_d*x_d+x*x_dd + y_d*y_d+y*y_dd)*2

def test_fun_M(Dx:float, Dy:float, r1:float, w1:float, r2:float, w2:float, Dphi:float) -> float:
    assert r1 >= 0
    assert r2 >= 0
    
    # x = Dx + r1*c1 - r2*c2
    x_max = Dx + r1 + r2
    x_min = Dx - r1 - r2
    
    # y = Dy + r1*s1 - r2*s2
    y_max = Dy + r1 + r2
    y_min = Dy - r1 - r2

    # x_d = -w1*r1*s1 + w2*r2*s2
    x_d_abs = r1*abs(w1) + r2*abs(w2)

    # y_d =  w1*r1*c1 - w2*r2*c2
    y_d_abs = x_d_abs

    # x_dd = -w1*w1*r1*c1 + w2*w2*r2*c2
    x_dd_abs = r1*w1*w1 + r2*w2*w2
    # y_dd = -w1*w1*r1*s1 + w2*w2*r2*s2
    y_dd_abs = x_dd_abs
    
    # return (x_d*x_d+x*x_dd + y_d*y_d+y*y_dd)*2
    xx_dd_candidates = (x_min*x_dd_abs,-x_min*x_dd_abs,x_max*x_dd_abs,-x_max*x_dd_abs)
    yy_dd_candidates = (y_min*y_dd_abs,-y_min*y_dd_abs,y_max*y_dd_abs,-y_max*y_dd_abs)
    
    output_upper = x_d_abs*x_d_abs + max(xx_dd_candidates) + y_d_abs*y_d_abs + max(yy_dd_candidates)
    output_lower = min(xx_dd_candidates) + min(yy_dd_candidates)
    
    output_abs = max(abs(output_upper),abs(output_lower))
    return 2*output_abs


#################### Brent global solver in Python ####################

def minimize_brent(left:float, right:float, 
        f:typing.Callable[[float|np.ndarray],float|np.ndarray],
        M:float,
        epsilon:float=1e-3,
        plotting:bool=False
    ) -> float:
    
    bootstrap_samples = np.linspace(left,right,num=30,endpoint=True)
    bootstrap_f = f(bootstrap_samples)
    
    best_i = np.argmin(bootstrap_f)
    best = bootstrap_f[best_i]
    best_loc = bootstrap_samples[best_i]
    
    if plotting:
        samples = np.arange(left,right,10*epsilon)
        f_samples = f(samples)
        f_min = np.min(f_samples)
        f_max = np.max(f_samples)
        f_height = f_max-f_min
    
    a2 = left
    A2 = f(left)
    if (A2 < best):
        best = A2
        best_loc = a2
    
    iter_count = 0
    
    # If f is concave, solution is immediate
    if (M <= 0):
        return best_loc
    
    # Stepping for interpolation
    a3 = a2 + np.sqrt((A2 - best + epsilon)/(0.5*M))
    while a2 < right:
        if plotting:
            plt.plot(samples,f_samples,label=f"Objective function")
            plt.hlines([best],[left],[right],colors='r',linestyles=':',label=f"Best current (y={best:.4f})")
            plt.scatter([best_loc],[best],c='k',label=f'Best value location (x={best_loc:.4f})')
            plt.vlines([a2,right],[f_min-0.2*f_height,f_min-0.2*f_height],[f_max+0.2*f_height,f_max+0.2*f_height],
                       colors='k',linestyles='--',
                       label=f"Current interval: [ {a2:.3f} , {a3:.3f} , {right:.3f} ]")
            plt.legend()
            plt.show()
        
        A3 = f(a3)
        
        if (A3 < best):
            best = A3
            best_loc = a3
            

        # Parabolla interpolation: P(x) = (M/2)*x² + b x + c
        # With P(a2) = f(a2) and P(a3) = f(a3)
            
        b = (A3-A2)/(a3-a2) - (M/2)*(a3+a2)
        c = A2 - b*a2 - (M/2)*a2*a2

        P_x_min   = -b/M
        P_min     = (M/2)*P_x_min*P_x_min + b*P_x_min + c
            
        # Check if parabolla is above best estimate on [a2,a3]
        if (a2 <= P_x_min and P_x_min <= a3):
            P_check = (P_min > best-epsilon)
            
        elif (P_x_min < a2): # P is increasing on [a2,a3]
            
            P_check = (A2 > best-epsilon)
        else: # (P_x_min > a3) # P is decreasing on [a2,a3]
            P_check = (A3 > best-epsilon)


        if (P_check):
            a2 = a3
            a3 = a2 + np.sqrt((A2 - best + epsilon)/(0.5*M))
            A2 = f(a2)
        else:
            a3 = (a2+a3)/2
            
        iter_count += 1


    print(f"Iter_count: {iter_count}")

    return best_loc


#################### Some optim method I am testing ####################

def myopt(f:typing.Callable[[float|np.ndarray],float|np.ndarray],
          f_d:typing.Callable[[float|np.ndarray],float|np.ndarray],
        a:float, b:float, M:float, eps:float=1e-6,
        plotting:bool=False) -> float:
    
    assert a < b
    assert b-a > eps
    
    if plotting:
        samples = np.arange(a,b,10*eps)
        f_samples = f(samples)
        f_min = np.min(f_samples)
        f_max = np.max(f_samples)
        f_height = f_max-f_min
    
    bootstrap_samples = np.linspace(a,b,num=30,endpoint=True)
    bootstrap_f = f(bootstrap_samples)
    
    best_i = np.argmin(bootstrap_f)
    best = bootstrap_f[best_i]
    best_loc = bootstrap_samples[best_i]
    
    iter_count = 0
    while(a<b and b-a > eps):
        if plotting:
            plt.plot(samples,f_samples,label=f"Objective function")
            plt.hlines([best],[a],[b],colors='r',linestyles=':',label=f"Best current (y={best:.4f})")
            plt.scatter([best_loc],[best],c='k',label=f'Best value location (x={best_loc:.4f})')
            plt.vlines([a,b],[f_min-0.2*f_height,f_min-0.2*f_height],[f_max+0.2*f_height,f_max+0.2*f_height],
                       colors='k',linestyles='--',
                       label=f"Current interval: [ {a:.3f} , {b:.3f} ]")
            plt.legend()
            plt.show()
            
        # Evaluate at both ends and improve minimum
        A = f(a)
        A_d = f_d(a)
        
        if A < best:
            best = A
            best_loc = a
            
        B = f(b)
        B_d = f_d(b)
        
        if B < best:
            best = B
            best_loc = b
        
        # Shrink on left side (a)
        Delta_a = A_d*A_d + 2*M*(A-best)
        step_a = (A_d+np.sqrt(Delta_a))/M
        if plotting:
            print(f"Step a: {step_a:.4f}\nf'(a)= {A_d:.3f}")
        
        step_a = max(step_a,eps)
        
        a += step_a
        
        # Shrink on right side (b)
        Delta_b = B_d*B_d + 2*M*(B-best)
        step_b = (B_d-np.sqrt(Delta_b))/M
        if plotting:
            print(f"Step b: {step_b:.4f}\nf'(b)= {B_d:.3f}")
            print("--------------------\n")
        
        step_b = min(step_b,-eps)
        
        b += step_b
        
        
        
        iter_count += 1
        
        
    print(f"Iter_count: {iter_count}")    
    
    return best_loc 

#################### A mix of the two ####################

def minimize_optbrent(left:float, right:float, 
        f:typing.Callable[[float|np.ndarray],float|np.ndarray],
        f_d:typing.Callable[[float|np.ndarray],float|np.ndarray],
        M:float,
        epsilon:float=1e-3,
        plotting:bool=False
    ) -> float:
    
    bootstrap_samples = np.linspace(left,right,num=30,endpoint=True)
    bootstrap_f = f(bootstrap_samples)
    
    best_i = np.argmin(bootstrap_f)
    best = bootstrap_f[best_i]
    best_loc = bootstrap_samples[best_i]
    
    if plotting:
        samples = np.arange(left,right,10*epsilon)
        f_samples = f(samples)
        f_min = np.min(f_samples)
        f_max = np.max(f_samples)
        f_height = f_max-f_min
    
    a2 = left
    A2 = f(left)
    A2_d = f_d(left)
    if (A2 < best):
        best = A2
        best_loc = a2
    
    iter_count = 0
    
    # If f is concave, solution is immediate
    if (M <= 0):
        return best_loc
    
    # Stepping for interpolation
    a3 = a2 + np.sqrt((A2 - best + epsilon)/(0.5*M))
    while a2 < right:
        
        # Shrink on left side (a)
        Delta_a = A2_d*A2_d + 2*M*(A2-best)
        step_a = (A2_d+np.sqrt(Delta_a))/M
        if plotting:
            print(f"Step a: {step_a:.4f}\nf'(a)= {A2_d:.3f}")
        
        step_a = max(step_a,epsilon)
        
        if plotting:
            plt.plot(samples,f_samples,label=f"Objective function")
            plt.hlines([best],[left],[right],colors='r',linestyles=':',label=f"Best current (y={best:.4f})")
            plt.scatter([best_loc],[best],c='k',label=f'Best value location (x={best_loc:.4f})')
            plt.vlines([a2,right],[f_min-0.2*f_height,f_min-0.2*f_height],[f_max+0.2*f_height,f_max+0.2*f_height],
                       colors='k',linestyles='--',
                       label=f"Current interval: [ {a2:.3f} , {a3:.3f} , {right:.3f} ]")
            plt.legend()
            plt.show()
        
        A3 = f(a3)
        
        if (A3 < best):
            best = A3
            best_loc = a3
            

        # Parabolla interpolation: P(x) = (M/2)*x² + b x + c
        # With P(a2) = f(a2) and P(a3) = f(a3)
            
        b = (A3-A2)/(a3-a2) - (M/2)*(a3+a2)
        c = A2 - b*a2 - (M/2)*a2*a2

        P_x_min   = -b/M
        P_min     = (M/2)*P_x_min*P_x_min + b*P_x_min + c
            
        # Check if parabolla is above best estimate on [a2,a3]
        if (a2 <= P_x_min and P_x_min <= a3):
            P_check = (P_min > best-epsilon)
            
        elif (P_x_min < a2): # P is increasing on [a2,a3]
            
            P_check = (A2 > best-epsilon)
        else: # (P_x_min > a3) # P is decreasing on [a2,a3]
            P_check = (A3 > best-epsilon)


        step_a3 = np.sqrt((A2 - best + epsilon)/(0.5*M))
        if (P_check):
            step_astar = a3-a2
            if step_astar > step_a:                
                a2 = a3
            else:
                a2 += step_a
            
            a3 = a2 + step_a3
            A2 = f(a2)
            A2_d = f_d(a2)
        else:
            step_astar = (a3-a2)/2
            
            if step_astar > step_a:    
                a3 = (a2+a3)/2
            else:
                a2 += step_a
                a3 = a2 + step_a3
                A2 = f(a2)
                A2_d = f_d(a2)
                
        if plotting:
            print(f"Taylor step: {step_a:.3f}")
            print(f"Brent  step: {step_astar:.3f}\n")
        
                
            
        iter_count += 1


    print(f"Iter_count: {iter_count}")

    return best_loc

#################### Entrypoint ####################

def main():
    
    
    Dx = 2
    Dy = 3
    r1 = 1
    r2 = 1.3
    w1 = 5
    w2 = r1*w1/r2
    
    Dphi = np.pi/6
    
    eps = 1e-3
    a = -10
    b =  10
    
    p = np.polynomial.Polynomial([0.374,34.2,786,-1.34e3,117,367,10.7,-31-3.06+0.751+0.1])
    p_d = p.deriv(1)
    
    f   = p#lambda t : test_fun(Dx,Dy,r1,w1,r2,w2,Dphi,t)
    f_d = p_d#lambda t : test_fun_d(Dx,Dy,r1,w1,r2,w2,Dphi,t)
    # M = test_fun_M(Dx,Dy,r1,w1,r2,w2,Dphi)
    
    samples = np.arange(a,b,eps)
    f_samples = f(samples)
    s_min = np.argmin(f_samples)
    min_loc = samples[s_min]
    
    p_dd = p_d.deriv(1)
    M = max(np.max(p_dd(samples)),-np.min(p_dd(samples)))
    

    
    
    
    print("-- Start MyOpt --")
    min_solve = myopt(f,f_d,a,b,M,eps,plotting=False)
    
    print("-- Start Brent --")
    min_brent = minimize_brent(a,b,f,M,eps,plotting=False)
    
    print("-- Start OptBrent --")
    min_optbrent = minimize_optbrent(a,b,f,f_d,M,eps,plotting=False)
    
    print("\n---- Results -----\n")
    print(f"Sampling minimum at : {min_loc:.5f}")
    print(f"Sampling minimum val: {f(min_loc):.5f}\n")
    print(f"Optim minimum at    : {min_solve:.5f}")
    print(f"Optim minimum val   : {f(min_solve):.5f}\n")
    print(f"Brent minimum at    : {min_brent:.5f}")
    print(f"Brent minimum val   : {f(min_brent):.5f}\n")
    print(f"OptBrent minimum at : {min_optbrent:.5f}")
    print(f"OptBrent minimum val: {f(min_optbrent):.5f}")
    
    plt.plot(samples,f_samples,label=f"Test function")
    plt.hlines([f(min_loc)],[a],[b],  label=f"Sampling min : {f(min_loc):.5f}",colors='k',linestyles=':')
    plt.scatter([min_loc],[f(min_loc)],     label=f"Sampling loc : {min_loc:.5f}",c='k')
    plt.hlines([f(min_solve)],[a],[b],label=f"Optim min    : {f(min_solve):.5f}",colors='r',linestyles='--')
    plt.scatter([min_solve],[f(min_solve)], label=f"Optim loc    : {min_solve:.5f}",c='r')
    plt.hlines([f(min_brent)],[a],[b],label=f"Brent min    : {f(min_solve):.5f}",colors='g',linestyles='--')
    plt.scatter([min_brent],[f(min_brent)], label=f"Brent loc    : {min_solve:.5f}",c='r')
    plt.legend()
    plt.show()
    
if __name__ == '__main__':
    main()
    