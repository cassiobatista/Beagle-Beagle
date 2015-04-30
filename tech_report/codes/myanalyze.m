% Apr 2015: Cassio Trindade Batista
% myanalyze: Converts vectors of durations into step functions and plot'em all

function y = myanalyze(a, b, c, d)

debug = false;
is_vec = [~isvector(a) ~isvector(b) ~isvector(c) ~isvector(d)];
min_len = [length(a)<35 length(b)<35 length(c)<35 length(d)<35];
if sum([sum(is_vec) sum(min_len)]) ~= 0
	error('Error')
end

% threshold: expecting specify a enough range to get the entire signal
myend = 43;

% Append All vector into one cell
% inside the range, extracts exactly the signal that matters without repeating
x = {a, b, c, d};
for i=1:length(x)
	[mx, idx] = max(x{i}(2:myend));
	x{i} = x{i}(2:idx);
end

% Converting from vector of durations into step function
% if a piece lasts n usecs then it'll become a step with n ones or zeros
y = {[], [], [], []};
for i=1:length(x)
	for j=1:length(x{i})
		if rem(j, 2) == 1 % odd .:. high peak
			y{i} = [y{i} ones(1, x{i}(j))];
		else % even .:. low peak
			y{i} = [y{i} zeros(1, x{i}(j))];
		end
	end
end

% plot' em all
myend = 23000;
color = {'b', 'r', 'g', 'k'};
figure;
for i=1:length(y)
	subplot(length(y), 1, i);
	plot(y{i}, color{i}, 'LineWidth', 5);
	xlim([0 myend]); set(gca, 'XTick', [0:2000:myend]); grid on;
end

% return the mean of the duration of all signals
% y = mean([sum(a) sum(b) sum(c) sum(d)])/1000;
