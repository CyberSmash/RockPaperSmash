//
// Created by chrisnoper on 6/4/23.
//

#ifndef ROCKPAPERSCISSORS_POINT_H
#define ROCKPAPERSCISSORS_POINT_H


class Point {
public:
    Point();
    Point(int row, int col);
    Point& operator=(const Point& other) = default;
    int row;
    int col;
};


#endif //ROCKPAPERSCISSORS_POINT_H
