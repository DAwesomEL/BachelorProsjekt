%% RIGHT manipulator


% Forward kinematics

clear; clc; close all;
L1 = 75/1000;   %m
L2 = 150/1000;  %m
L3 = 140/1000;  %m
L4 = 210/1000;  %m
L5 = 200/1000;  %m

theta = deg2rad(30);

% digits(4)
% syms q1 q2 q3 q4;
% T2 = RotZ(q1+theta)*TransZ(L1)*RotX(pi/2) * RotZ(q2)*TransZ(L2+L3)*RotX(-pi/2) * RotZ(q3-theta)*RotX(-pi/2) * RotZ(q4)*TransZ(L4+L5);


digits(4)
syms q1 q2 q3;
T1 = RotZ(q1+theta)*TransZ(L1)*RotX(pi/2) * RotZ(q2)*TransZ(L2+L3)*RotX(-pi/2) * RotZ(q3-theta)*RotX(-pi/2)

FK_Right = vpa(T1(1:3,4),2);

X = FK_Right(1);
Y= FK_Right(2);
q = FK_Right(3);

% Inverse Kinematics

% Jacobian

xB = sum(T1(1,4))
yB = sum(T1(2,4))
zB = sum(T1(3,4))

J = jacobian([xB;yB;zB],[q1;q2;q3])

DOF_test = rank(J)




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


X = FK_Left(1)
Y= FK_Left(2)
q = FK_Left(3)



