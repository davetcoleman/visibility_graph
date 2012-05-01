clear
clc

data = csvread('data.cvs')

n = data(:,1)
logger1 = 50.*(n.^2) .* log ( n );
logger2 = 25.*(n.^2) .* log ( n );

loglog( data(:,1),data(:,2),'bo-', ...
    data(:,1),logger1,'k:',data(:,1),logger2,'k:')
    
    
set(gca,'FontSize',14)

legend('Number of lookups','O(n^2 log n)','Location','NorthWest')
xlabel('Input size, n')
ylabel('Number of operations, T')
title('Atomic Operations of Lee Visibility Graph Algorithm');

