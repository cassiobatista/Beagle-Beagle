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

% inside the range, extracts exactly the signal that matters without repeating
[m, id] = max(a(2:myend));
a = a(2:id);

[m, id] = max(b(2:myend));
b = b(2:id);

[m, id] = max(c(2:myend));
c = c(2:id);

[m, id] = max(d(2:myend));
d = d(2:id);

% Converting from vector of durations into step function
% if a piece lasts n usecs then it'll become a step with n ones or zeros
x_a = x_b = x_c = x_d = [];
for i=1:length(a)
	if rem(i, 2) == 1 % odd .:. high peak
		x_a = [x_a ones(1, a(i))];
	else % even .:. low peak
		x_a = [x_a zeros(1, a(i))];
	end
end

for i=1:length(b)
	if rem(i, 2) == 1 % odd .:. high peak
		x_b = [x_b ones(1, b(i))];
	else % even .:. low peak
		x_b = [x_b zeros(1, b(i))];
	end
end

for i=1:length(c)
	if rem(i, 2) == 1 % odd .:. high peak
		x_c = [x_c ones(1, c(i))];
	else % even .:. low peak
		x_c = [x_c zeros(1, c(i))];
	end
end

for i=1:length(d)
	if rem(i, 2) == 1 % odd .:. high peak
		x_d = [x_d ones(1, d(i))];
	else % even .:. low peak
		x_d = [x_d zeros(1, d(i))];
	end
end

% plot' em all
myend=23000;
figure;
subplot(4,1,1);
plot(x_a, 'b', 'LineWidth', 5);
xlim([0 myend]); set(gca, 'XTick', [0:2000:myend]); grid on; set(gca, 'YTick', [0:1]);
subplot(4,1,2);
plot(x_b, 'r', 'LineWidth', 5);
xlim([0 myend]); set(gca, 'XTick', [0:2000:myend]); grid on; set(gca, 'YTick', [0:1]);
subplot(4,1,3);
plot(x_c, 'g', 'LineWidth', 5);
xlim([0 myend]); set(gca, 'XTick', [0:2000:myend]); grid on; set(gca, 'YTick', [0:1]);
subplot(4,1,4);
plot(x_d, 'k', 'LineWidth', 5);
xlim([0 myend]); set(gca, 'XTick', [0:2000:myend]); grid on; set(gca, 'YTick', [0:1]);

% return the mean of the duration of all signals
y = mean([sum(a) sum(b) sum(c) sum(d)])/1000;
