#!/usr/bin/env Rscript

library(ggplot2)

# All included
data <- read.table("ml-100k_all_included.txt", header = TRUE)

data$k <- as.factor(data[,'k'])

data$method[data$method==0] <- 'Cosine'
data$method[data$method==1] <- 'L1'
data$method[data$method==2] <- 'L2'
data$method[data$method==3] <- 'PCC'

# MAE-K
p <- ggplot(data, aes(k, MAE))
p + geom_boxplot(aes(colour=method)) +
    labs(x='k', y='MAE', title='MAE-K Plot', colour='Method')
#ggsave("Plots/MAE_K.pdf")

# RMSE-K
p <- ggplot(data, aes(k, RMSE))
p + geom_boxplot(aes(colour=method)) +
    labs(x='k', y='RMSE', title='RMSE-K Plot', colour='Method')
#ggsave("Plots/RMSE_K.pdf")

# Recall-K
p <- ggplot(data, aes(k, Recall))
p + geom_boxplot(aes(colour=method)) + 
    labs(x='k', y='Recall', title='Recall-K Plot', colour='Method')
#ggsave("Plots/Recall_K.pdf")


#  Matched Only
data <- read.table("ml-100k_matched_only.txt", header = TRUE)

data$k <- as.factor(data[,'k'])

data$method[data$method==0] <- 'Cosine'
data$method[data$method==1] <- 'L1'
data$method[data$method==2] <- 'L2'
data$method[data$method==3] <- 'PCC'

# MAE-K
p <- ggplot(data, aes(k, MAE))
p + geom_boxplot(aes(colour=method)) +
    labs(x='k', y='MAE', title='MAE-K Plot (Matched Only)', colour='Method')

# RMSE-K
p <- ggplot(data, aes(k, RMSE))
p + geom_boxplot(aes(colour=method)) +
    labs(x='k', y='RMSE', title='RMSE-K Plot (Matched Only)', colour='Method')

# Recall-K
p <- ggplot(data, aes(k, Recall))
p + geom_boxplot(aes(colour=method)) +
    labs(x='k', y='Recall', title='Recall-K Plot (Matched Only)', colour='Method')
