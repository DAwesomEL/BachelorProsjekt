function [Tnew] = killSmall(T)
%KILLSMALL Summary of this function goes here
%   Detailed explanation goes here
Tnew = T;
Tnew(1,2) = 0;
Tnew(2,2) = 0;
Tnew(3,3) = 0;
end

