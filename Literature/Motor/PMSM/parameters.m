%% Electrical parameters of PMSM motor 
p = 6;
n_p=3;
T_rated=0.8; %Nm
n_rated=4500; %rpm
I_rated = 1.4; %A, rms
J = 0.0334; %kg.m^2
% Calculated using SimulationX model:
R_s = 5.05; %Ohm
L_s = 0.0173; %H

%% Calculated parameters:
Psi_R= 3*T_rated/sqrt(2)/p/I_rated;

%% Transformation matrices:
T32 = [1 -1/2 -1/2; 0 sqrt(3)/2 -sqrt(3)/2 ];
T23 = [2/3 0; -1/3 1/sqrt(3); -1/3 -1/sqrt(3)];

Ampmax = 400*sqrt(2)/sqrt(3)*3/2;
%% Current Controller
ki = 100;
kp = 1000;

