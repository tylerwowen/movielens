//
//  similarity_measurement_tests.cpp
//  moivelens
//
//  Created by Tyler Weimin Ouyang on 3/23/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include <stdio.h>
#include "gtest/gtest.h"

#define private public
#include "neighborsfinder.hpp"

namespace {
  
  // The fixture for testing class Foo.
  class NeighborsLocatorTest : public ::testing::Test {
  protected:
    NeighborsLocator *sut;
    
    NeighborsLocatorTest() {
      sut = new NeighborsLocator(NULL, 4, 5, 0);
    }
    
    virtual ~NeighborsLocatorTest() {
      delete sut;
    }
  };
  
  // MARK: euclideanDistance
  TEST_F(NeighborsLocatorTest, euclideanDistance__identicalVectors__return0) {
    Ratings_list r1 = {{1,3}, {2,4}},
    r2 = {{1,3}, {2,4}};
    sut->numOfItems = 2;
    
    double distance = sut->euclideanDistance(r1, r2);
    EXPECT_DOUBLE_EQ(0.0, distance);
  }
  
  TEST_F(NeighborsLocatorTest, euclideanDistance__distance_is_2) {
    Ratings_list r1 = {{1,3}, {2,4}, {3,3}, {4,4}},
    r2 = {{1,4}, {2,3}, {3,4}, {4,3}};
    
    double distance = sut->euclideanDistance(r1, r2);
    EXPECT_DOUBLE_EQ(2, distance);
    distance = sut->euclideanDistance(r2, r1);
    EXPECT_DOUBLE_EQ(2, distance);
  }
  
  TEST_F(NeighborsLocatorTest, euclideanDistance__different_length) {
    Ratings_list r1 = {{1,3}, {2,4}, {3,3}, {4,4}},
    r2 = {{1,4}, {2,3}, {3,4}};
    
    double distance = sut->euclideanDistance(r1, r2);
    EXPECT_DOUBLE_EQ(sqrt(19), distance);
    
    distance = sut->euclideanDistance(r2, r1);
    EXPECT_DOUBLE_EQ(sqrt(19), distance);
  }
  
  TEST_F(NeighborsLocatorTest, euclideanDistance__complex_input) {
    Ratings_list r1 = {{1,3}, {2,4}, {4,3}, {6,4}},
    r2 = {{1,4}, {6,3}, {7,4}};
    sut->numOfItems = 7;
    
    double distance = sut->euclideanDistance(r1, r2);
    EXPECT_DOUBLE_EQ(sqrt(43), distance);
    
    distance = sut->euclideanDistance(r2, r1);
    EXPECT_DOUBLE_EQ(sqrt(43), distance);
  }
  
  // MARK: cityBlockDistance
  TEST_F(NeighborsLocatorTest, cityBlockDistance__identicalVectors__return0) {
    Ratings_list r1 = {{1,3}, {2,4}},
    r2 = {{1,3}, {2,4}};
    sut->numOfItems = 2;
    
    double distance = sut->cityBlockDistance(r1, r2);
    EXPECT_DOUBLE_EQ(0.0, distance);
  }
  
  TEST_F(NeighborsLocatorTest, cityBlockDistance__distance_is_4) {
    Ratings_list r1 = {{1,3}, {2,4}, {3,3}, {4,4}},
    r2 = {{1,4}, {2,3}, {3,4}, {4,3}};
    
    double distance = sut->cityBlockDistance(r1, r2);
    EXPECT_DOUBLE_EQ(4, distance);
    distance = sut->cityBlockDistance(r2, r1);
    EXPECT_DOUBLE_EQ(4, distance);
  }
  
  TEST_F(NeighborsLocatorTest, cityBlockDistance__different_length) {
    Ratings_list r1 = {{1,3}, {2,4}, {3,3}, {4,4}},
    r2 = {{1,4}, {2,3}, {3,4}};
    
    double distance = sut->cityBlockDistance(r1, r2);
    EXPECT_DOUBLE_EQ(7, distance);
    
    distance = sut->cityBlockDistance(r2, r1);
    EXPECT_DOUBLE_EQ(7, distance);
  }
  
  TEST_F(NeighborsLocatorTest, cityBlockDistance__complex_input) {
    Ratings_list r1 = {{1,3}, {2,4}, {4,3}, {6,4}},
    r2 = {{1,4}, {6,3}, {7,4}};
    sut->numOfItems = 7;
    
    double distance = sut->cityBlockDistance(r1, r2);
    EXPECT_DOUBLE_EQ(13, distance);
    
    distance = sut->cityBlockDistance(r2, r1);
    EXPECT_DOUBLE_EQ(13, distance);
  }
  
  // MARK: pcc
  TEST_F(NeighborsLocatorTest, pcc__identicalVectors__return_1) {
    Ratings_list r1 = {{1,3}, {2,4}},
    r2 = {{1,3}, {2,4}};
    sut->numOfItems = 2;
    
    double distance = sut->pcc(r1, r2);
    EXPECT_DOUBLE_EQ(1, distance);
  }
  
  TEST_F(NeighborsLocatorTest, pcc__distance_is_4) {
    Ratings_list r1 = {{1,3}, {2,4}, {3,3}, {4,4}},
    r2 = {{1,4}, {2,3}, {3,4}, {4,3}};
    
    double distance = sut->pcc(r1, r2);
    EXPECT_DOUBLE_EQ(-1, distance);
    distance = sut->pcc(r2, r1);
    EXPECT_DOUBLE_EQ(-1, distance);
  }
  
  TEST_F(NeighborsLocatorTest, pcc__different_length) {
    Ratings_list r1 = {{1,3}, {2,4}, {3,3}, {5,4}},
    r2 = {{1,4}, {2,3}, {3,4}};
    sut->numOfItems = 5;
    
    double distance = sut->pcc(r1, r2);
    EXPECT_NEAR(13.0/(9*sqrt(14)), distance, 0.0000001);
    
    distance = sut->pcc(r2, r1);
    EXPECT_NEAR(13.0/(9*sqrt(14)), distance, 0.0000001);
  }
  
  TEST_F(NeighborsLocatorTest, pcc__complex_input) {
    Ratings_list r1 = {{1,3}, {2,4}, {4,3}, {6,4}},
    r2 = {{1,4}, {6,3}, {7,4}};
    sut->numOfItems = 7;
    
    double distance = sut->pcc(r1, r2);
    EXPECT_DOUBLE_EQ(sqrt(7.0/913), distance);
    
    distance = sut->pcc(r2, r1);
    EXPECT_DOUBLE_EQ(sqrt(7.0/913), distance);
  }
  
  TEST_F(NeighborsLocatorTest, pcc__complex_input2) {
    Ratings_list r1 = {{1,1}, {2,3}, {3,2}, {5,4}, {7,4}, {8,2}, {10,3}},
    r2 = {{1,1}, {2,2}, {3,3}, {6,1}, {7,3}, {9,1}, {10,3}};
    sut->numOfItems = 10;
    
    double distance = sut->pcc(r1, r2);
    EXPECT_DOUBLE_EQ(7.4/sqrt(22.9*14.4), distance);
    
    distance = sut->pcc(r2, r1);
    EXPECT_DOUBLE_EQ(7.4/sqrt(22.9*14.4), distance);
  }
  
  // MARK: cosineSimilarity
  TEST_F(NeighborsLocatorTest, cosineSimilarity__identicalVectors__return_1) {
    Ratings_list r1 = {{1,3}, {2,4}},
    r2 = {{1,3}, {2,4}};
    sut->numOfItems = 2;
    
    double distance = sut->cosineSimilarity(r1, r2);
    EXPECT_DOUBLE_EQ(1, distance);
  }
  
  TEST_F(NeighborsLocatorTest, cosineSimilarity__distance_is_4) {
    Ratings_list r1 = {{1,3}, {2,4}, {3,3}, {4,4}},
    r2 = {{1,4}, {2,3}, {3,4}, {4,3}};
    
    double distance = sut->cosineSimilarity(r1, r2);
    EXPECT_DOUBLE_EQ(0.96, distance);
    distance = sut->cosineSimilarity(r2, r1);
    EXPECT_DOUBLE_EQ(0.96, distance);
  }
  
  TEST_F(NeighborsLocatorTest, cosineSimilarity__different_length) {
    Ratings_list r1 = {{1,3}, {2,4}, {3,3}, {5,4}},
    r2 = {{1,4}, {2,3}, {3,4}};
    sut->numOfItems = 5;
    
    double distance = sut->cosineSimilarity(r1, r2);
    EXPECT_DOUBLE_EQ(36/sqrt(50*41), distance);
    
    distance = sut->cosineSimilarity(r2, r1);
    EXPECT_DOUBLE_EQ(36/sqrt(50*41), distance);
  }
  
  TEST_F(NeighborsLocatorTest, cosineSimilarity__complex_input) {
    Ratings_list r1 = {{1,3}, {2,4}, {4,3}, {6,4}},
    r2 = {{1,4}, {6,3}, {7,4}};
    sut->numOfItems = 7;
    
    double distance = sut->cosineSimilarity(r1, r2);
    EXPECT_DOUBLE_EQ(24/sqrt(50*41), distance);
    
    distance = sut->cosineSimilarity(r2, r1);
    EXPECT_DOUBLE_EQ(24/sqrt(50*41), distance);
  }

}