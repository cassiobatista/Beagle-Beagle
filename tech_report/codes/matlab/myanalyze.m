% Apr 2015: Cassio Trindade Batista
% myanalyze: Converts vectors of durations into step functions and plot'em all
function y = myanalyze(x)

% Check if input matches with the required type
if (iscell(x) && length(x)<6)
	for i=1:length(x)
		if (~isvector(x{i}) || length(x{i})<35)
			error('Error');
		end
	end
else
	error('Error');
end
	
% threshold: expects to specify a enough range to get the entire signal ONCE
myend = 43;

%% 2 consecutive maxs represent the beginning of the signal, that starts idle
%% Inside this range, extracts exactly the signal that matters without repeating
%for i=1:length(x)
%	if length(x{i}) > myend
%		[mx, idx] = max(x{i}(2:myend));
%		if mx > 2000
%			x{i} = x{i}(2:idx);
%		end
%	else
%		x{i} = x{i}(2:end);
%	end
%end

x{1}=x{1}(2:end);
x{2}=x{2}(2:end);
x{3}=x{3}(2:end);
x{4}=x{4}(2:end);

% Converting from vector of durations x into step function y
% If a piece lasts n us then it'll become a step with n high or low values
y = {[], [], [], []};
% for each vector into the cell ...
for i=1:length(x) 
	% for each value of the vector from the cell ...
	for j=1:length(x{i})
		% check the index of the value
		if rem(j, 2) == 1 % if it's odd .:. high peak
			y{i} = [y{i} ones(1, x{i}(j))]; % fill with n ones
		else % otherwise, if it's even  .:. low peak
			y{i} = [y{i} zeros(1, x{i}(j))]; % fill with n zeros
		end
	end
end

% Plot' em all
myend = 60000;
color = {'b', 'r', 'g', 'k', 'm', 'y', 'c'};
figure;
for i=1:length(y)
	subplot(length(y), 1, i);
	plot(y{i}, color{i}, 'LineWidth', 5);
	xlim([0 myend]); set(gca, 'XTick', [0:2000:myend]); grid on;
end
%%% EOF %%%
