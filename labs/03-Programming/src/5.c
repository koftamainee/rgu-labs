#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../../libs/errors.h"

// Errors
#define INVALID_POLYGON (-1)

#define POLYGON_DOTS_COUNT (5)

typedef struct {
  double x;
  double y;
} dot;

int is_convex(int points_count, int *ans, ...);
void cross_product(dot point_1, dot point_2, dot point_3, double *ans);

int program_03_5(int argc, char *argv[]) {
  int err, ans, i;
  dot polygon[POLYGON_DOTS_COUNT];

  srand((unsigned int)time(NULL));
  printf("Polygon initialization:\n");
  for (i = 0; i < POLYGON_DOTS_COUNT; ++i) {
    polygon[i].x = (rand() % 21 - 10) + (((double)(rand() % 1001)) / 1000);
    polygon[i].y = (rand() % 21 - 10) + (((double)(rand() % 1001)) / 1000);

    // polygon[i].x = i - 1;
    // polygon[i].y = i + 1;
    printf("Dot %d: x = %.3lf, y = %.3lf\n", i, polygon[i].x, polygon[i].y);
  }

  err = is_convex(POLYGON_DOTS_COUNT, &ans, polygon[0], polygon[1], polygon[3],
                  polygon[4]);
  switch (err) {
  case OK:
    if (ans) {
      printf("Polygon is convex\n");
    } else {
      printf("Polygon is not convex\n");
    }
    break;
  case DEREFERENCING_NULL_PTR:
    printf("Dereferencing NULL ptr\n");
    break;
  case INVALID_POLYGON:
    printf("Invalid polygon coords have been passed\n");
    break;
  default:
    printf("Undefined behavior 0_o\nError code: %d\n", err);
  }
  return err;
}

int is_convex(int points_count, int *ans, ...) {
  double cross;
  int err, i, orientation;
  dot vectors[3];
  va_list valist;

  if (ans == NULL) {
    return DEREFERENCING_NULL_PTR;
  }

  if (points_count < 3) {
    return INVALID_POLYGON;
  }
  va_start(valist, ans);

  orientation = 0;

  for (i = 0; i < points_count - 2; ++i) {
    if (i == 0) {
      vectors[0] = va_arg(valist, dot);
      vectors[1] = va_arg(valist, dot);
      vectors[2] = va_arg(valist, dot);
    } else {
      vectors[0] = vectors[1];
      vectors[1] = vectors[2];
      vectors[2] = va_arg(valist, dot);
    }
    cross_product(vectors[0], vectors[1], vectors[2], &cross);

    if (cross != 0) {
      if (orientation == 0) {
        orientation = (cross > 0) ? 1 : -1;
      } else if ((cross > 0 && orientation == -1) ||
                 (cross < 0 && orientation == 1)) {
        *ans = 0;
        va_end(valist);
        return OK;
      }
    }
  }
  *ans = 1;
  va_end(valist);
  return OK;
}

void cross_product(dot point_1, dot point_2, dot point_3, double *ans) {
  *ans = (point_2.x - point_1.x) * (point_3.x - point_1.x) -
         (point_2.y - point_1.y) * (point_3.y - point_1.y);
}