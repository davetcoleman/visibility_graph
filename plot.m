clear
clc

data = csvread('data_space.cvs')

n = data(:,1)
logger1 = 7.*(n.^1);
logger2 = 3.*(n.^1);

loglog( data(:,1),data(:,2),'bo-', ...
    data(:,1),logger1,'k:',data(:,1),logger2,'k:')
    
    
set(gca,'FontSize',14)

legend('Numerical Results','O(n)','Location','NorthWest')
xlabel('Input size, n')
ylabel('Space Usage, S')
title('Atomic Space Usage of Lee Visibility Graph Algorithm');

