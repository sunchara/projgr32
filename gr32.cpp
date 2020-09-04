#include "gr32.h"


BOOL WLine(HDC hdc, int x1, int y1, int x2, int y2)
{
    MoveToEx(hdc, x1, y1, NULL); //сделать текущими координаты x1, y1
    return LineTo(hdc, x2, y2); //нарисовать линию
}


BOOL WPrintText(HDC hdc, int x1, int y1,  std::string  s)
{
    return TextOut(hdc, x1, y1, &(s[0]) , s.length());
}

BOOL __PrintText(HDC hdc, int x1, int y1,char *text)
{
    return TextOut(hdc, x1,y1, text , strlen(text));
}


///================================================================================
///
///          class Window32  STATIC MEMBERS AND METHODS
///
///================================================================================


HINSTANCE               Window32::hThisInstance;
HINSTANCE               Window32::hPrevInstance;
LPSTR                   Window32::lpszArgument;
int                     Window32::nCmdShow;

WNDCLASSEX              Window32::wincl;// window class


//Window32**              Window32::WndList;
//int                     Window32::Wnd_num;
std::stringstream      Window32::Window32LogStream;
std::vector<Window32*> Window32::wlist;

std::string Window32::GetLog()
{
    return Window32LogStream.str();
}
std::string Window32::GetAndCleanLog()
{
    std::string s=Window32LogStream.str();
    Window32LogStream.str(std::string());
    Window32LogStream.clear();
    return s;
}


ATOM Window32::RegisterClassWindow32(HINSTANCE _hThisInstance, HINSTANCE _hPrevInstance,
                                     LPSTR _lpszArgument, int _nCmdShow)
{
    Window32::hThisInstance=_hThisInstance;
    Window32::hPrevInstance=_hPrevInstance;
    Window32::lpszArgument=_lpszArgument;
    Window32::nCmdShow=_nCmdShow;


    Window32::wincl.hInstance = hThisInstance;
    Window32::wincl.lpszClassName = "Window32gr";
    Window32::wincl.lpfnWndProc = Window32::Window32Proc;
    Window32::wincl.style = CS_HREDRAW|CS_VREDRAW;
    Window32::wincl.cbSize = sizeof (WNDCLASSEX);

    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = ((HBRUSH)GetStockObject(WHITE_BRUSH));


    return RegisterClassEx (&wincl);
}


void Window32::RegisterWindow32()
{
    wlist.push_back(this);
}

Window32* Window32::FindWindow32(HWND _hWnd)
{
    for(int i=0; i<Window32::wlist.size(); ++i)
        if((Window32::wlist[i])->destroyed==false && (Window32::wlist[i])->hWnd==_hWnd)
            return Window32::wlist[i];


    return NULL;
}

///================================================================================
///
///          class Window32  CONSTRUCTOR AND DISPLAY OUTPUT COFIGURATION
///
///================================================================================
Window32::Window32()
{
    focus_width=0.5;
    focus_height=0.5;

    width=0;
    height=0;

    Ifocus=I2vector(0,0);
    Rfocus=R2vector(0,0);

    is_mappig_generated=false;

    destroyed=false;
    show_grid=true;

    RegisterWindow32();
}


void Window32::GenerateMappings(double x1,double x2,double y1,double y2)
{
    double _k_x,_k_y;
    if(fabs(x1-x2)<1e-10 || fabs(y1-y2)<1e-10)
    {
        k_x=1;
        k_y=-1;
        return;
    }
    Rfocus=R2vector((x1+x2)/2,(y1+y2)/2);
    _k_x= 2* fmin(width  *focus_width,  width *(1-focus_width)) /fabs(x1-x2);
    _k_y= 2* fmin(height *focus_height, height*(1-focus_height))/fabs(y1-y2);
    k_x= fmax(_k_x,_k_y);
    k_y=-fmax(_k_x,_k_y);
    k_x=1./k_x;
    k_y=1./k_y;
    is_mappig_generated=true;
}


void Window32::GenerateMappings(const R2vector & _show_top, const R2vector & _show_bottom)
{
    GenerateMappings(_show_top.x, _show_bottom.x, _show_top.y, _show_bottom.y);
}


void Window32::SetDisplayOptions(const R2vector & _show_top, const R2vector & _show_bottom,double _focus_width,double _focus_height)
{
    show_top=_show_top;
    show_bottom=_show_bottom;

    focus_width=_focus_width;
    focus_height=_focus_height;

    //GenerateMappings(show_top,show_bottom);
}
void Window32::SetDisplayOptions(double x1,double x2,double y1,double y2,double _focus_width,double _focus_height)
{
    this->Window32::SetDisplayOptions(R2vector(x1,y1),R2vector(x2,y2),_focus_width,_focus_height);
}
HWND Window32::Create(TCHAR name[])
{
    this->hWnd = CreateWindowEx (
                     0,                                           // Extended possibilites for variation
                     Window32::wincl.lpszClassName,                         // Classname
                     _T(name),                                // Title Text
                     WS_OVERLAPPEDWINDOW,                         // default window
                     CW_USEDEFAULT,                               // Windows decides the position
                     CW_USEDEFAULT,                               // where the window ends up on the screen
                     900,                                         // The programs width
                     600,                                         // and height in pixels
                     HWND_DESKTOP,                                // The window is a child-window to desktop
                     NULL,                                        // No menu
                     Window32::hThisInstance,                               // Program Instance handler
                     NULL                                         // No Window Creation data
                 );

    /* Make the window visible on the screen */
    ShowWindow (this->hWnd, nCmdShow);
    UpdateWindow(this->hWnd);
    return this->hWnd;

}


///================================================================================
///
///          class Window32  WINDOW CONTROLS
///
///================================================================================
void Window32::Zoom(double k)
{
    k_x*=k;
    k_y*=k;

}
void Window32::Moove(double horizontal,double vertical)
{
    R2vector dv=RScreenDiaganal();
    dv.x*=horizontal;
    dv.y*=vertical;
    Rfocus=Rfocus+dv;
}

void Window32::Moove(const I2vector & dx)
{
    Rfocus=ItoR(dx);
}
void Window32::Moove(const R2vector & dx)
{
    Rfocus=Rfocus+dx;
}


///================================================================================
///
///          class Window32  DISPLAY SPACE INFORMATION
///
///================================================================================


bool Window32::IIsInsideScreen(const I2vector &v)
{
    return (-width*focus_width<=v.x && v.x<=width*(1-focus_width) &&
            -height*focus_height<=v.y && v.y<=height*(1- focus_height));
}
bool Window32::RIsInsideScreen(const R2vector &v)
{
    return IIsInsideScreen(RtoI(v));
}
I2vector Window32::IScreenTop()
{
    return I2vector(-width*focus_width,-height*focus_height);
}
I2vector Window32::IScreenDiaganal()
{
    return I2vector(width,height);
}



R2vector Window32::RScreenTop()
{
    return ItoR(I2vector(-width*focus_width,height*(1-focus_height)));
}

R2vector Window32::RScreenDiaganal()
{
    return ItoR(I2vector(width*(1-focus_width),-height*focus_height))-RDownLeft();
}
I2vector Window32::IDownLeft()
{
    return I2vector(-width*focus_width,height*(1-focus_height));
}
I2vector Window32::IDownRight()
{
    return I2vector(width*(1-focus_width),height*(1-focus_height));
}

I2vector Window32::IUpLeft()
{
    return I2vector(-width*(focus_width),-height*(focus_height));
}

I2vector Window32::IUpRight()
{
    return I2vector(width*(1-focus_width),-height*(focus_height));
}

R2vector Window32::RDownLeft()
{
    return ItoR(IDownLeft());
}
R2vector Window32::RDownRight()
{
    return ItoR(IDownRight());
}
R2vector Window32::RUpLeft()
{
    return ItoR(IUpLeft());
}
R2vector Window32::RUpRight()
{
    return ItoR(IUpRight());
}


///================================================================================
///
///          class Window32  PAINTING METHODTS
///
///================================================================================
void Window32::PickPen(int r,int g,int b,int width,int style)
{
    DeleteObject(hPen);
    hPen=CreatePen(style,width,RGB(r,g,b));
    SelectObject(hdc,hPen);
}
void Window32::IEllipse(const I2vector &x1,const I2vector &x2)
{
    Ellipse(hdc,x1.x,x1.y,x2.x,x2.y);
}
void Window32::ICircle(const I2vector &x1,int r)
{
    IEllipse(x1+I2vector(-r,-r),x1+I2vector(r,r));
}

void Window32::REllipse(const R2vector &x1,const R2vector &x2)
{
    I2vector _x1(RtoI(x1)),_x2(RtoI(x2));
    Ellipse(hdc,_x1.x,_x1.y,_x2.x,_x2.y);
}
void Window32::RCircle(const R2vector &x1,double r)
{
    I2vector _x1(RtoI(x1+R2vector(-r,-r))), _x2(RtoI(x1+R2vector(r,r)));
    Ellipse(hdc,_x1.x,_x1.y,_x2.x,_x2.y);
}

void Window32::ILine(const I2vector &x1,const I2vector &x2)
{
    WLine(hdc,x1.x, x1.y, x2.x, x2.y);
}
void Window32::RLine(const R2vector &x1,const R2vector &x2)
{
    ILine(this->RtoI(x1),this->RtoI(x2));
}
void Window32::IPrintText(const I2vector& x1,  std::string s)
{
    WPrintText(hdc,x1.x ,x1.y, s);
}
void Window32::RPrintText(const R2vector& x1,  std::string s)
{
    IPrintText(RtoI(x1),s);
}




void Window32::DrawGrid2()
{

    PickPen(100,100,100);
    int i;
    double grid_step=1e-10;

    while(grid_step<50*k_x)
        grid_step*=10;

    if(grid_step<25*k_x)
        grid_step*=2;


    R2vector grid_step_x(grid_step,0);
    R2vector grid_step_y(0,grid_step);

    R2vector grid_start(roundl(RDownLeft().x/grid_step+1)*grid_step,
                        roundl(RDownLeft().y/grid_step+1)*grid_step);

    R2vector pos;
    R2vector from,to;

    std::stringstream ss;
    if(RIsInsideScreen(grid_start-grid_step_x))
        i=-1;
    else
        i=0;

    for(pos=grid_start+i*grid_step_x; RIsInsideScreen(pos) ; ++i, pos=grid_start+i*grid_step_x)
    {
        from.x=pos.x;
        from.y=RDownLeft().y;

        to.x=pos.x;
        to.y=(RUpRight()).y;

        RLine(from,to);


        ss<<pos.x;
        IPrintText(RtoI(pos)+I2vector(5,10),ss.str());
        ss.str(std::string());
        ss.clear();

    }
    if(RIsInsideScreen(grid_start-grid_step_y))
        i=-1;
    else
        i=0;
    for(pos=grid_start+i*grid_step_y; RIsInsideScreen(pos) ; ++i, pos=grid_start+i*grid_step_y)
    {
        from.x=RDownLeft().x;
        from.y=pos.y;

        to.x=(RUpRight()).x;
        to.y=pos.y;

        RLine(from,to);

        ss<<pos.y;
        IPrintText(RtoI(pos)+I2vector(-30,-17),ss.str());
        ss.str(std::string());
        ss.clear();
    }
    PickPen(100,100,100,3);
    if(RIsInsideScreen(R2vector(0,Rfocus.y)))
    {
        from.x=0;
        from.y=RDownLeft().y;

        to.x=0;
        to.y=RUpRight().y;
        RLine(from,to);
    }

    if(RIsInsideScreen(R2vector(Rfocus.x,0)))
    {
        from.x=RDownLeft().x;
        from.y=0;

        to.x=RUpRight().x;
        to.y=0;
        RLine(from,to);
    }


}
void Window32::DrawGrid(std::string x_Axis_name,std::string y_Axis_name,double x_Axis_koef,double y_Axis_koef)
{

    PickPen(100,100,100);
    int i;
    double grid_step_xx=1e-10;

    while(grid_step_xx<50*fabs(k_x))
        grid_step_xx*=10;

    if(grid_step_xx<25*k_x)
        grid_step_xx*=2;

    double grid_step_yy=1e-10;

    while(grid_step_yy<50*fabs(k_y))
        grid_step_yy*=10;
    if(grid_step_yy<25*k_x)
        grid_step_yy*=2;

    R2vector grid_step_x(grid_step_xx,0);
    R2vector grid_step_y(0,grid_step_yy);

    R2vector grid_start(roundl(RDownLeft().x/grid_step_xx+1)*grid_step_xx,
                        roundl(RDownLeft().y/grid_step_yy+1)*grid_step_yy);

    R2vector pos;
    R2vector from,to;

    std::stringstream ss;
    if(RIsInsideScreen(grid_start-grid_step_x))
        i=-1;
    else
        i=0;

    for(pos=grid_start+i*grid_step_x; RIsInsideScreen(pos) ; ++i, pos=grid_start+i*grid_step_x)
    {
        from.x=pos.x;
        from.y=RDownLeft().y;

        to.x=pos.x;
        to.y=(RUpRight()).y;

        RLine(from,to);


        ss<<(pos.x*x_Axis_koef);
        IPrintText(RtoI(pos)+I2vector(5,10),ss.str());
        ss.str(std::string());
        ss.clear();

    }

    IPrintText(RtoI(R2vector(RDownRight().x, grid_start.y))+I2vector(-80,-20),x_Axis_name);


    if(RIsInsideScreen(grid_start-grid_step_y))
        i=-1;
    else
        i=0;
    for(pos=grid_start+i*grid_step_y; RIsInsideScreen(pos) ; ++i, pos=grid_start+i*grid_step_y)
    {
        from.x=RDownLeft().x;
        from.y=pos.y;

        to.x=(RUpRight()).x;
        to.y=pos.y;

        RLine(from,to);

        ss<<(pos.y*y_Axis_koef);
        IPrintText(RtoI(pos)+I2vector(-30,-17),ss.str());
        ss.str(std::string());
        ss.clear();
    }

    IPrintText(RtoI(R2vector(grid_start.x, RUpLeft().y))+I2vector(10,20),y_Axis_name);

    PickPen(100,100,100,2);
    if(RIsInsideScreen(R2vector(0,Rfocus.y)))
    {
        from.x=0;
        from.y=RDownLeft().y;

        to.x=0;
        to.y=RUpRight().y;
        RLine(from,to);
    }

    if(RIsInsideScreen(R2vector(Rfocus.x,0)))
    {
        from.x=RDownLeft().x;
        from.y=0;

        to.x=RUpRight().x;
        to.y=0;
        RLine(from,to);
    }


}


///================================================================================
///
///          class Window32  INIT, START AND END PAINTING
///
///================================================================================

bool Window32::StartPaint()
{
    if(!is_mappig_generated)
    {
        if(width<=0 || height<=0)
            return false;
        GenerateMappings(show_top,show_bottom);
    }

    hdc = BeginPaint(hWnd, &ps);
    //hdc= GetDC(this->hWnd);

    SetMapMode(hdc,MM_TEXT );
    SetViewportOrgEx(hdc, width*focus_width, height*focus_height, NULL); //Начало координат
    //SetViewportExtEx(hdc, width, -height, NULL); //Определяем облась вывода
    return true;

}

void Window32::FinishPaint()
{
    ValidateRect(hWnd, NULL);
    EndPaint(hWnd, &ps);
}


///================================================================================
///
///          class Window32  METHODS TO DO IN WINPROC
///
///================================================================================

void Window32::Update()
{
    InvalidateRect(hWnd, NULL, TRUE);
}

void Window32::Resize(int _width,int _height)
{
    width=_width;
    height=_height;
}

void Window32::Paint()
{
    this->PickPen(200,20,10);
    this->ILine(IDownLeft(),IUpRight());
    PickPen(0,225,10,3);
    RCircle(R2vector(0,0),30);
    DrawGrid();
    return;
}

bool Window32::ProcessEvent(UINT messg,WPARAM wParam, LPARAM lParam)
{
    return false;
}




int Window32::ProcessMessages()
{
    MSG messages;
    int wnd_quit_count,i;
    while (true)
    {
        if(GetMessage (&messages, NULL, 0, 0)==0)
        {
            wnd_quit_count=0;
            for(i=0; i<Window32::wlist.size(); ++i)
                if((Window32::wlist[i])->destroyed==true)
                    wnd_quit_count++;

            if(wnd_quit_count>=Window32::wlist.size())
                break;
        }

        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }
    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;

}
void Window32::Print2(TCHAR filename[])
{
    HDC hdcScreen;
    HDC hdcWindow;
    HDC hdcMemDC = NULL;
    HBITMAP hbmScreen = NULL;
    BITMAP bmpScreen;

    // Retrieve the handle to a display device context for the client
    // area of the window.
    hdcScreen = GetDC(NULL);
    hdcWindow = GetDC(hWnd);

    // Create a compatible DC which is used in a BitBlt from the window DC
    hdcMemDC = CreateCompatibleDC(hdcWindow);


    // Get the client area for size calculation
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);

    //This is the best stretch mode
    SetStretchBltMode(hdcWindow,HALFTONE);

    //The source DC is the entire screen and the destination DC is the current window (HWND)
    StretchBlt(hdcWindow,
               0,0,
               rcClient.right, rcClient.bottom,
               hdcScreen,
               0,0,
               GetSystemMetrics (SM_CXSCREEN),
               GetSystemMetrics (SM_CYSCREEN),
               SRCCOPY);

    // Create a compatible bitmap from the Window DC
    hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right-rcClient.left, rcClient.bottom-rcClient.top);


    // Select the compatible bitmap into the compatible memory DC.
    SelectObject(hdcMemDC,hbmScreen);

    // Bit block transfer into our compatible memory DC.
    BitBlt(hdcMemDC,
           0,0,
           rcClient.right-rcClient.left, rcClient.bottom-rcClient.top,
           hdcWindow,
           0,0,
           SRCCOPY);

    // Get the BITMAP from the HBITMAP
    GetObject(hbmScreen,sizeof(BITMAP),&bmpScreen);

    BITMAPFILEHEADER   bmfHeader;
    BITMAPINFOHEADER   bi;

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmpScreen.bmWidth;
    bi.biHeight = bmpScreen.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

    // Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that
    // call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc
    // have greater overhead than HeapAlloc.
    HANDLE hDIB = GlobalAlloc(GHND,dwBmpSize);
    char *lpbitmap = (char *)GlobalLock(hDIB);

    // Gets the "bits" from the bitmap and copies them into a buffer
    // which is pointed to by lpbitmap.
    GetDIBits(hdcWindow, hbmScreen, 0,
              (UINT)bmpScreen.bmHeight,
              lpbitmap,
              (BITMAPINFO *)&bi, DIB_RGB_COLORS);

    // A file is created, this is where we will save the screen capture.
    HANDLE hFile = CreateFile(filename,
                              GENERIC_WRITE,
                              0,
                              NULL,
                              CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);

    // Add the size of the headers to the size of the bitmap to get the total file size
    DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    //Offset to where the actual bitmap bits start.
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

    //Size of the file
    bmfHeader.bfSize = dwSizeofDIB;

    //bfType must always be BM for Bitmaps
    bmfHeader.bfType = 0x4D42; //BM

    DWORD dwBytesWritten = 0;
    WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

    //Unlock and Free the DIB from the heap
    GlobalUnlock(hDIB);
    GlobalFree(hDIB);

    //Close the handle for the file that was created
    CloseHandle(hFile);

    //Clean up
done:
    DeleteObject(hbmScreen);
    DeleteObject(hdcMemDC);
    ReleaseDC(NULL,hdcScreen);
    ReleaseDC(hWnd,hdcWindow);

    return;
}
void Window32::Print(TCHAR filename[])
{
    HDC _tmp_store=this->hdc;

    HDC hdc_=GetDC(this->hWnd);
    HDC hDCMem = CreateCompatibleDC(hdc_);

    this->hdc=CreateCompatibleDC(hdc_);
    HBITMAP hBmp = CreateCompatibleBitmap(hdc_, this->width, this->height);
    SelectObject(this->hdc, hBmp);
    RECT rect;


    SetMapMode(this->hdc,MM_TEXT );
    SetViewportOrgEx(this->hdc, width*focus_width, height*focus_height, NULL); //Начало координат

    GetWindowRect(this->hWnd, & rect);
    rect.top    =IDownLeft().y  +  3*IScreenDiaganal().y;
    rect.bottom =IUpRight().y   -  3*IScreenDiaganal().y;

    rect.right  =IUpRight().x   -  3*IScreenDiaganal().y;
    rect.left   =IDownLeft().x  +  3*IScreenDiaganal().y;
    FillRect(this->hdc,&rect,wincl.hbrBackground);


    this->Paint();

    ReleaseDC(this->hWnd,this->hdc);

    FILE*fp=NULL;
    LPVOID pBuf=NULL;
    BITMAPINFO bmpInfo;
    BITMAPFILEHEADER bmpFileHeader;
    do
    {
        ZeroMemory(&bmpInfo,sizeof(BITMAPINFO));
        bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
        GetDIBits(hdc_,hBmp,0,0,NULL,&bmpInfo,DIB_RGB_COLORS);

        if(bmpInfo.bmiHeader.biSizeImage<=0)
            bmpInfo.bmiHeader.biSizeImage=bmpInfo.bmiHeader.biWidth*abs(bmpInfo.bmiHeader.biHeight)*(bmpInfo.bmiHeader.biBitCount+7)/8;

        if((pBuf = malloc(bmpInfo.bmiHeader.biSizeImage))==NULL)
        {
            MessageBox( NULL, "Unable to Allocate Bitmap Memory", "Error", MB_OK|MB_ICONERROR);
            break;
        }
        bmpInfo.bmiHeader.biCompression=BI_RGB;
        GetDIBits(hdc_,hBmp,0,bmpInfo.bmiHeader.biHeight,pBuf, &bmpInfo, DIB_RGB_COLORS);

        if((fp = fopen(filename,"wb"))==NULL)
        {
            MessageBox( NULL, "Unable to Create Bitmap File", "Error", MB_OK|MB_ICONERROR);
            break;
        }
        bmpFileHeader.bfReserved1=0;
        bmpFileHeader.bfReserved2=0;
        bmpFileHeader.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+bmpInfo.bmiHeader.biSizeImage;
        bmpFileHeader.bfType='MB';
        bmpFileHeader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

        fwrite(&bmpFileHeader,sizeof(BITMAPFILEHEADER),1,fp);
        fwrite(&bmpInfo.bmiHeader,sizeof(BITMAPINFOHEADER),1,fp);
        fwrite(pBuf,bmpInfo.bmiHeader.biSizeImage,1,fp);
    }
    while(false);

    if(hdc_)ReleaseDC(NULL,hdc_);
    if(pBuf) free(pBuf);
    if(fp)fclose(fp);

}

///================================================================================
///
///     class Window32  WINPROC FUNCTION
///
///================================================================================

LRESULT CALLBACK Window32::Window32Proc(HWND hWnd, UINT messg,
                                        WPARAM wParam, LPARAM lParam)
{
    char ch;
    //double r=200;
    //int d=100;

    int fwKeys ;
    int zDelta;
    int xPos;
    int yPos;

    double moove_x,moove_y,zoom;
    static int scrshot_number=1;
    static std::stringstream ss;
    static std::string str;
    static I2vector mouse_pos;
    static bool LMouseButtonDown=false;

    Window32 *wnd=Window32::FindWindow32(hWnd);
    if(wnd==NULL)
        return(DefWindowProc(hWnd, messg, wParam, lParam));

    if(wnd->ProcessEvent(messg,wParam,lParam)==true)
        return 0;

    switch(messg)
    {
    case WM_CREATE:

        break;

    case WM_SIZE:
        wnd->Resize(LOWORD(lParam),HIWORD(lParam));
        wnd->Update();
        break;
//сообщение рисования
    case WM_PAINT:
        if(!wnd->StartPaint())
            return 0;
        wnd->Paint();
        wnd->FinishPaint();

        break;
    case WM_KEYDOWN:
        moove_x=moove_y=0;
        switch (wParam)
        {
        case VK_LEFT:
            moove_x=-0.1;
            break;
        case VK_RIGHT:
            moove_x=0.1;
            break;
        case VK_UP:
            moove_y=0.1;
            break;
        case VK_DOWN:
            moove_y=-0.1;
            break;
        }
        wnd->Moove(moove_x,moove_y);

        wnd->Update();

        break;

    case WM_CHAR:
        ch=(TCHAR)wParam;
        zoom=1;
        switch (ch)
        {
        case 'z':
            zoom=0.8;
            break;
        case 'x':
            zoom=1/0.8;
            break;
        case 'p':
            wnd->Update();
            ss.clear();
            ss.str(std::string());
            ss<<scrshot_number<<".bmp";
            str=ss.str();
            perr("Screenshot saved to "<<str);
            wnd->Print(&(str[0]));
            ss.clear();
            ss.str(std::string());
            scrshot_number++;
            break;

        }
        wnd->Zoom(zoom);
        wnd->Update();

        break;
    case WM_MOUSEWHEEL:
        //fwKeys = GET_KEYSTATE_WPARAM(wParam);
        zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        xPos = GET_X_LPARAM(lParam);
        yPos = GET_Y_LPARAM(lParam);

        wnd->Zoom(exp(-zDelta/500.));
        wnd->Update();
        break;
    case WM_LBUTTONDOWN:
        xPos = GET_X_LPARAM(lParam);
        yPos = GET_Y_LPARAM(lParam);
        mouse_pos=I2vector(xPos,yPos);
        LMouseButtonDown=true;
        break;
    case WM_MOUSEMOVE:
        if(LMouseButtonDown==true)
        {
            xPos = GET_X_LPARAM(lParam);
            yPos = GET_Y_LPARAM(lParam);
            wnd->Moove(mouse_pos-I2vector(xPos,yPos));
            mouse_pos=I2vector(xPos,yPos);
            wnd->Update();
        }
        break;
    case WM_LBUTTONUP:


        LMouseButtonDown=false;
        break;
    case WM_PRINTCLIENT:
        if(!wnd->StartPaint())
            return 0;
        wnd->Paint();
        wnd->FinishPaint();
        break;
    //сообщение выхода - разрушение окна
    case WM_DESTROY:
        wnd->destroyed=true;
        PostQuitMessage(0); //Посылаем сообщение выхода с кодом 0 - нормальное завершение
        break;

    default:
        return(DefWindowProc(hWnd, messg, wParam, lParam)); //освобождаем очередь приложения от нераспознаных
    }
    return 0;
}


