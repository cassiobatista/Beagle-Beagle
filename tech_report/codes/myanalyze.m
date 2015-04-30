% Apr 2015: Cassio Trindade Batista
% myanalyze: Converts vectors of durations into step functions and plot'em all
function y = myanalyze(a, b, c, d)

is_vec = [~isvector(a) ~isvector(b) ~isvector(c) ~isvector(d)];
min_len = [length(a)<35 length(b)<35 length(c)<35 length(d)<35];
if sum([sum(is_vec) sum(min_len)]) ~= 0
	error('Error')
end

% threshold: expecting specify a enough range to get the entire signal ONCE
myend = 43;

% Append All vector into one cell
% Inside the range, extracts exactly the signal that matters without repeating
x = {a, b, c, d};
for i=1:length(x)
	[mx, idx] = max(x{i}(2:myend));
	if mx > 2000
		x{i} = x{i}(2:idx);
	end
end

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
myend = 23000;
color = {'b', 'r', 'g', 'k'};
figure;
for i=1:length(y)
	subplot(length(y), 1, i);
	plot(y{i}, color{i}, 'LineWidth', 5);
	xlim([0 myend]); set(gca, 'XTick', [0:2000:myend]); grid on;
end
