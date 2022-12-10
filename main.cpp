
#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <cmath>

  //Declare Windows procedure
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

//Make the class name into a global variable
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;                //This is the handle for our window
    MSG messages;            // Here messages to the application are saved
    WNDCLASSEX wincl;         //Data structure for the windowclass

//     The Window structure
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      // This function is called by windows
    wincl.style = CS_DBLCLKS;                  //Catch double-clicks
    wincl.cbSize = sizeof (WNDCLASSEX);

  //   Use default icon and mouse-pointer
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                  //No menu
    wincl.cbClsExtra = 0;                       //No extra bytes after the window class
    wincl.cbWndExtra = 0;                       //structure or the window instance
//    Use Windows default colour as the background of the window
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    // Register the window class, and if it fails quit the program
    if (!RegisterClassEx (&wincl))
        return 0;

    // The class is registered, lets create the program
       hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Code::Blocks Template Windows App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               544,                 /* The programs width */
               375,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );
    // Make the window visible on the screen
    ShowWindow (hwnd, nCmdShow);

     //Run the message loop. It will run until GetMessage() returns 0
        while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

void swap(int& xs,int& ys,int & xe,int& ye)
{
    int temp = xs;
    xs = xe;
    xe = temp;

    int newTmp = ys;
    ys = ye;
    ye = newTmp;
}

void drawLine(HDC hdc, int xs, int ys, int xe, int ye)
{
    int dx = xe - xs;
    int dy = ye - ys;

    SetPixel(hdc,xs,ys,RGB(0,0,0));
    if(dx  > dy)
    {
        if(xs>xe)
        {
            swap(xs,ys,xe,ye);
        }
        int x = xs;
        double y = ys;
        double m = (double)dy/(double)dx;
        while(x<xe)
        {
            y+=m;
            x++;
            SetPixel(hdc,x,y,RGB(0,0,0));
        }
    }
    else
    {
        if(ys>ye)
        {
            swap(xs,ys,xe,ye);
        }
        int y = ys;
        double x = xs;
        double mi = (double)dx/(double)dy;
        while(y<ye)
        {
            y++;
            x+=mi;
            SetPixel(hdc,x,y,RGB(0,0,0));
        }
    }
}

bool check = true;
void Draw8Points(HDC hdc,int xc,int yc, int m, int n,COLORREF color)
{
    SetPixel(hdc, xc+m, yc+n, color);
    SetPixel(hdc, xc-m, yc+n, color);
    SetPixel(hdc, xc-m, yc-n, color);
    SetPixel(hdc, xc+m, yc-n, color);
    if(check)
    {
        drawLine(hdc,xc+m,yc+n,xc-m,yc-n);
    }
    SetPixel(hdc, xc+n, yc+m, color);
    SetPixel(hdc, xc-n, yc+m, color);
    SetPixel(hdc, xc-n, yc-m, color);
    SetPixel(hdc, xc+n, yc-m, color);

}

int round(double& x)
{
    return ((int)(x+0.5));

}

void drawCircle(HDC hdc,int xc,int yc, double r,COLORREF color)
{
    int x=0,y=r;
    Draw8Points(hdc,xc,yc,x,y,color);
    while(x<y)
    {
        x++;
        y = round(sqrt((double)(r*r-x*x)));
        Draw8Points(hdc,xc,yc,x,y,color);
    }
}
  //This function is called by the Windows function DispatchMessage()
int setRadius(int xc, int yc, int x, int y)
{
    int r = round(sqrt(pow(x-xc,2.0) + pow(y-yc,2.0)));
    return r;
}


int arr[4];
int xc1, yc1, x,y, xc2, yc2;

double r1,r2;
int i = 0;
int newX,newY;
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    HDC hdc=GetDC(hwnd);
    switch (message)                  // handle the messages
    {
    case WM_LBUTTONDBLCLK :
        newX = LOWORD(lParam);
        newY = HIWORD(lParam);
        arr[i] = newX;
        arr[++i] = newY;
        i++;

        break;
    case WM_RBUTTONDBLCLK :
        x = LOWORD(lParam);
        y = HIWORD(lParam);
        r1 = setRadius(arr[0],arr[1],x,y);
        r2 = setRadius(arr[2],arr[3],x,y);
        drawCircle(hdc,arr[0],arr[1],r1,RGB(0,0,0));
        check = false;
        drawCircle(hdc,arr[2],arr[3],r2,RGB(0,0,0));
        break;
    case WM_DESTROY:
        PostQuitMessage (0);  //      send a WM_QUIT to the message queue
        break;
    default  :   //                  for messages that we dont deal with
        return DefWindowProc (hwnd, message, wParam, lParam);

    }

    return 0;
}
