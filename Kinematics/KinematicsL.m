%% LEFT manipulator
clear; clc; close all;
%L1 = 0.29;
%L2 = 0.15;
%L3 = 0.15;
%L4 = 0.20;
%L5 = 0.1;

theta = deg2rad(-30)


digits(4)



syms q1 q2 q3 q4 L1 L2 L3 L4 L5;
T = RotZ(-q1-theta)*TransZ(L1)*RotX(-pi/2) * RotZ(-q2)*TransZ(L2+L3)*RotX(pi/2) * RotZ(-q3+theta)*RotX(pi/2) * RotZ(-q4)*TransZ(L4+L5);

FK_Left = vpa(T(1:3,4),2);