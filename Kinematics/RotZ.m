function [rotateZ] = RotZ(angleZ)
%ROTX Summary of this function goes here
%   Detailed explanation goes here
c = cos(angleZ);
s = sin(angleZ);
rotateZ = [c -s 0 0; s c 0 0; 0 0 1 0; 0 0 0 1];
end

