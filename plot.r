#!/usr/bin/env Rscript

library(ggplot2)
pdf( "Rplots.pdf", width = 12, height = 8 )

prepareData <- function(name) {
  data <- read.table(name, header = TRUE)

  data$k <- as.factor(data[,'k'])
  data$method[data$method==0] <- 'Cosine'
  data$method[data$method==1] <- 'L1'
  data$method[data$method==2] <- 'L2'
  data$method[data$method==3] <- 'PCC'

  data$matched_only[data$matched_only==0] <- 'All_included'
  data$matched_only[data$matched_only==1] <- 'Matched_only'
  return(data)
}

plot_all <- function(dataDir1, dataDir2, size) {
  data1 = prepareData(dataDir1)
  data2 = prepareData(dataDir2)
  data = rbind(data1, data2)

  # MAE-K
  p <- ggplot(data, aes(k, MAE))
  p <- p + geom_boxplot(aes(colour=method)) +
      labs(title=paste('MAE-K Plot', size, sep='__'), colour='Method') +
      facet_grid(. ~ matched_only)
  print(p)

  # RMSE-K
  p <- ggplot(data, aes(k, RMSE))
  p <- p + geom_boxplot(aes(colour=method)) +
      labs(title=paste('RMSE-K Plot', size, sep='__'), colour='Method') +
      facet_grid(. ~ matched_only)
  print(p)

  # Recall-K
  p <- ggplot(data, aes(k, Recall))
  p <- p + geom_boxplot(aes(colour=method)) +
      labs(title=paste('Recall-K Plot', size, sep='__'), colour='Method') +
      facet_grid(. ~ matched_only)
  print(p)

  # Time-K
  p <- ggplot(data, aes(k, time))
  p <- p + geom_boxplot(aes(colour=method)) +
      labs(title=paste('Time-K Plot', size, sep='__'), colour='Method') +
      facet_grid(. ~ matched_only)
  print(p)
}

# 100k
plot_all('raw_results/ml-100k_all_included.txt', 'raw_results/ml-100k_matched_only.txt', '100k');

# 1m
plot_all('raw_results/ml-1m_all_included.txt', 'raw_results/ml-1m_matched_only.txt', '1m');
