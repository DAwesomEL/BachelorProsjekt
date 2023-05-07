function [translationX] = TransX(distX)
%TRANSX Summary of this function goes here
%   Detailed explanation goes here
translationX = [1 0 0 distX; 0 1 0 0; 0 0 1 0; 0 0 0 1];
end

