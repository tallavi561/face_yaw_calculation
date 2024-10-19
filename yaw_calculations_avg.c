#include <stdio.h>
#include <math.h>

// פונקציה לחישוב קו שעובר דרך שתי נקודות
void line_from_points(float p1_x, float p1_y, float p2_x, float p2_y, float *A, float *B, float *C) {
    if (p1_x == p2_x){
      *A = -1;
      *B = 0;
      *C = p1_x;
    } else {
      float m = (p1_y - p2_y)/(p1_x - p2_x);
      *C = p1_y - m * p1_x;
      *A = m;
      *B = -1;
    }
}

// פונקציה לחישוב נקודת החיתוך בין שני קווים
int intersection_point(float A1, float B1, float C1, float A2, float B2, float C2, float *x, float *y) {
    float determinant = A1 * B2 - A2 * B1;

    if (determinant == 0) {
        return 0; // אין חיתוך
    } else {
        // A1 = 
        *x = -(B2 * C1 - B1 * C2) / determinant;
        *y = -(A1 * C2 - A2 * C1) / determinant;
        return 1; // יש חיתוך
    }
}

// פונקציה לחישוב Q ו-YAW
void calculate_yaw_and_Q(float *eye_points, float *nose_points, float *Q, float *yaw) {
    // חישוב נקודות ממוצעות של קצה שמאלי וימני של העיניים
    float RIGHT_eye_x = 0, RIGHT_eye_y = 0, LEFT_eye_x = 0, LEFT_eye_y = 0;

    // ממוצע של שלוש הנקודות השמאליות ביותר
    for (int i = 0; i < 6; i++) {
        RIGHT_eye_x += eye_points[i * 2];
        RIGHT_eye_y += eye_points[i * 2 + 1];
    }
    RIGHT_eye_x /= 6;
    RIGHT_eye_y /= 6;
    printf("Avg of left eye: (%f, %f)\n", RIGHT_eye_x, RIGHT_eye_y);
    // ממוצע של שלוש הנקודות הימניות ביותר
    for (int i = 6; i < 12; i++) {
        LEFT_eye_x += eye_points[i * 2];
        LEFT_eye_y += eye_points[i * 2 + 1];
    }
    LEFT_eye_x /= 6;
    LEFT_eye_y /= 6;
    printf("Avg of right eye: (%f, %f)\n", LEFT_eye_x, LEFT_eye_y);
    printf("Nose: (%f, %f) (%f, %f)\n",nose_points[0], nose_points[1], nose_points[4], nose_points[5]);

    // חישוב קו העיניים
    float A1, B1, C1;
    line_from_points(RIGHT_eye_x, RIGHT_eye_y, LEFT_eye_x, LEFT_eye_y, &A1, &B1, &C1);
    printf("line of eyes: A1=%f, B1=%f, C1=%f\n",A1, B1, C1 );
    // חישוב קו האף
    float A2, B2, C2;
    line_from_points(nose_points[0], nose_points[1], nose_points[4], nose_points[5], &A2, &B2, &C2);
    printf("line of nose: A2=%f, B2=%f, C2=%f\n",A2, B2, C2 );

    // חישוב נקודת החיתוך
    float intersection_x, intersection_y;
    
    if (!intersection_point(A1, B1, C1, A2, B2, C2, &intersection_x, &intersection_y)) {
        *Q = 0;
        *yaw = 0;
        return; // אין חיתוך
    }
    printf("Intersection: (%f, %f)\n", intersection_x, intersection_y);
    // חישוב Q על פי הנוסחה
    *Q = sqrt((  (RIGHT_eye_x - intersection_x) * (RIGHT_eye_x - intersection_x) + 
            (RIGHT_eye_y - intersection_y) * (RIGHT_eye_y - intersection_y)) / 
            ((intersection_x - LEFT_eye_x) * (intersection_x - LEFT_eye_x) + 
            (intersection_y - LEFT_eye_y) * (intersection_y - LEFT_eye_y)));

      // חישוב Yaw
      float tan_a = (*Q - 1) / ((*Q + 1)) * 1.3923;
      *yaw = atan(tan_a) * 180.0 / M_PI;

//     // חישוב Q
//     *Q = fabs((right_eye_x - intersection_x) / (intersection_x - left_eye_x));

//     // חישוב YAW
//     double tan_a = (*Q - 1) / ((*Q + 1)) * 1.607;
//     *yaw = atan(tan_a) * 180.0 / M_PI;
      printf("Q: %f, yaw: %f\n", *Q, *yaw);
}

// פונקציה שמחשבת את ממוצע שלושת ה-X וה-Y הגבוהים ביותר
void calculate_average(int arr[12], double *X_star, double *Y_star) {
    // מגדירים מערך להחזיק את שלושת ה-X הגבוהים ביותר
    int top_X[3] = {0};
    int top_Y[3] = {0};

    // עוברים על כל הזוגות ומעדכנים את שלושת ה-X וה-Y הגבוהים ביותר
    for (int i = 0; i < 6; i++) {
        int current_X = arr[i * 2];
        int current_Y = arr[i * 2 + 1];

        // בודקים אם ה-X הנוכחי גבוה מאחד משלושת ה-X הגבוהים ביותר
        for (int j = 0; j < 3; j++) {
            if (current_X > top_X[j]) {
                // מזיזים את ה-X וה-Y הגבוהים ביותר אחד אחורה
                for (int k = 2; k > j; k--) {
                    top_X[k] = top_X[k - 1];
                    top_Y[k] = top_Y[k - 1];
                }
                // מכניסים את ה-X וה-Y החדשים
                top_X[j] = current_X;
                top_Y[j] = current_Y;
                break;
            }
        }
    }

    // מחשבים ממוצע של שלושת ה-X ושלושת ה-Y הגבוהים ביותר
    *X_star = (top_X[0] + top_X[1] + top_X[2]) / 3.0;
    *Y_star = (top_Y[0] + top_Y[1] + top_Y[2]) / 3.0;
}

void calculate_yaw_and_Q2(float *eye_points, float *nose_points, float *Q, float *yaw) {
    // חישוב נקודות ממוצעות של קצה שמאלי וימני של העיניים
    float RIGHT_eye_x = 0, RIGHT_eye_y = 0, LEFT_eye_x = 0, LEFT_eye_y = 0;

    // ממוצע של שלוש הנקודות השמאליות ביותר
    for (int i = 0; i < 6; i++) {
        RIGHT_eye_x += eye_points[i * 2];
        RIGHT_eye_y += eye_points[i * 2 + 1];
    }
    RIGHT_eye_x /= 6;
    RIGHT_eye_y /= 6;
    printf("Avg of left eye: (%f, %f)\n", RIGHT_eye_x, RIGHT_eye_y);
    // ממוצע של שלוש הנקודות הימניות ביותר
    for (int i = 6; i < 12; i++) {
        LEFT_eye_x += eye_points[i * 2];
        LEFT_eye_y += eye_points[i * 2 + 1];
    }
    LEFT_eye_x /= 6;
    LEFT_eye_y /= 6;
    printf("Avg of right eye: (%f, %f)\n", LEFT_eye_x, LEFT_eye_y);
    printf("Nose: (%f, %f) (%f, %f)\n",nose_points[0], nose_points[1], nose_points[4], nose_points[5]);

    // חישוב קו העיניים
    float A1, B1, C1;
    line_from_points(RIGHT_eye_x, RIGHT_eye_y, LEFT_eye_x, LEFT_eye_y, &A1, &B1, &C1);
    printf("line of eyes: A1=%f, B1=%f, C1=%f\n",A1, B1, C1 );
    // חישוב קו האף
    float A2, B2, C2;
    line_from_points(nose_points[0], nose_points[1], nose_points[4], nose_points[5], &A2, &B2, &C2);
    printf("line of nose: A2=%f, B2=%f, C2=%f\n",A2, B2, C2 );

    // חישוב נקודת החיתוך
    float intersection_x, intersection_y;
    
    if (!intersection_point(A1, B1, C1, A2, B2, C2, &intersection_x, &intersection_y)) {
        *Q = 0;
        *yaw = 0;
        return; // אין חיתוך
    }
    printf("Intersection: (%f, %f)\n", intersection_x, intersection_y);
    // חישוב Q על פי הנוסחה
    *Q = sqrt((  (RIGHT_eye_x - intersection_x) * (RIGHT_eye_x - intersection_x) + 
            (RIGHT_eye_y - intersection_y) * (RIGHT_eye_y - intersection_y)) / 
            ((intersection_x - LEFT_eye_x) * (intersection_x - LEFT_eye_x) + 
            (intersection_y - LEFT_eye_y) * (intersection_y - LEFT_eye_y)));

      // חישוב Yaw
      float tan_a = (*Q - 1) / ((*Q + 1)) * 1.3923;
      *yaw = atan(tan_a) * 180.0 / M_PI;

//     // חישוב Q
//     *Q = fabs((right_eye_x - intersection_x) / (intersection_x - left_eye_x));

//     // חישוב YAW
//     double tan_a = (*Q - 1) / ((*Q + 1)) * 1.607;
//     *yaw = atan(tan_a) * 180.0 / M_PI;
      printf("Q: %f, yaw: %f\n", *Q, *yaw);
}




int main() {
    // דוגמה של קלט
    float eye_points[24] = {
        30, 40, 30, 40, 30, 40, 30, 40, 30, 40, 30, 40, // עין ימין
        50, 40, 50, 40, 50, 40, 50, 40, 50, 40, 50, 40  // עין שמאל
    };

    float nose_points[6] = { 45, 50, 45, 51, 45, 52 };

    // משתנים לאחסון התוצאה
    float Q, yaw;

    // חישוב
    calculate_yaw_and_Q(eye_points, nose_points, &Q, &yaw);

    // תוצאה
    printf("Q: %f, Yaw: %f\n", Q, yaw);

    return 0;
}
