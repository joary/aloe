% /* 
%  * Copyright (c) 2012-2013, Ismael Gomez-Miguelez <ismael.gomez@tsc.upc.edu>.
%  * This file is part of ALOE++ (http://flexnets.upc.edu/)
%  * 
%  * ALOE++ is free software: you can redistribute it and/or modify
%  * it under the terms of the GNU Lesser General Public License as published by
%  * the Free Software Foundation, either version 3 of the License, or
%  * (at your option) any later version.
%  * 
%  * ALOE++ is distributed in the hope that it will be useful,
%  * but WITHOUT ANY WARRANTY; without even the implied warranty of
%  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%  * GNU Lesser General Public License for more details.
%  * 
%  * You should have received a copy of the GNU Lesser General Public License
%  * along with ALOE++.  If not, see <http://www.gnu.org/licenses/>.
%  */

% Author:       Vuk Marojevic (vuk.marojevic@gmail.com)
% Last revision:16.1.2013 

% Hard modulation demapping QPSK
% ==============================
% zero indicates the value assigned to '0', and one the value assigned to '1'

% INPUTS
% in:       input vector of complex symbols
% zero, one:      real value, e.g. -100, 100, onto which a '0' and '1' is mapped
%
% OUTPUTS
% out:      output matrix of length(in) rows and 2 colums, where each
%           element represents a (soft or hard decision) bit
% 
function out = hard_qpsk(in, zero, one)

M = length(in);
out = -1*ones(M,2);

for i=1:M
    
    if (real(in(i)) > 0)  % right half-plane
        out(i,1) = zero;  % '0'
    else
        out(i,1) = one;	  % '1'
    end
    
    if (imag(in(i)) > 0)  % right half-plane
        out(i,2) = zero;  % '0'
    else
        out(i,2) = one;   % '1'
    end

end
