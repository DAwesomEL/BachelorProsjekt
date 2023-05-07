function [rotateX] = RotX(angleX)
%ROTX Summary of this function goes here
%   Detailed explanation goes here
c = cos(angleX);
s = sin(angleX);
rotateX = [1 0 0 0; 0 c -s 0; 0 s c 0; 0 0 0 1];
end

