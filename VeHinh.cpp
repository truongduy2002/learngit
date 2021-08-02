 
	#include <graphics.h>

#include <stdio.h>
#include "VeHinh.h"
#include "windows.h"
#include "mmsystem.h"

int thu_tu_mau_sac[] = {
  1,
  2,
  3
};

int so_hat_soi = 0;
int hangsoi[100];

const int STATE_BEGIN = 0;
const int STATE_ADD = 1;
const int STATE_ORDER = 2;
int state = STATE_ADD;
int mat_nuoc = -1; // mat nuoc dang o sau phan tu nao?
int drawing_color = BLACK;

int default_radius = 30; // ban kinh cua moi hat dau trong hangdau
int default_duong_kinh = default_radius * 2;
int start_point_x = 100; // toa do x cuar hat dau dau tien trong hangdau
int start_point_y = 100; // toa do y cua hat dau dau tien trong hangdau
int default_margin = 10; // khoang cach cac hat dau trong hangdau
char *status = "Hay them cac vien soi";
char *button_text = "NEXT";

bool dachondo = false;
bool dachonvang = false;
bool dachontrang = false;
int tong_da_chon = 0; // tong so cac mau da chon trong luc chon thu tu
int thu_tu_sap_xep[3]; // 3 1 2 => trang->do->vang
int vitri = 0; // trong luc chon hat dau, minh phai tang vi tri de biet hat dau cuoi cung o vi tri nao cua mang

void draw_beautiful_start_button() {
	char *chu_muon_viet = button_text;
	int do_dai_cua_chu = textwidth(chu_muon_viet);
	int do_cao_cua_chu = textheight(chu_muon_viet);
	outtextxy(600-(do_dai_cua_chu/2), 100 - (do_cao_cua_chu/2), chu_muon_viet);
	rectangle(500, 50, 700, 150);
}

void draw_status() // ham xuat chu
{
	outtext(status);
}



// check whether (x,y) is inside rectangle or not?
bool is_inside_rectangle(int x, int y, int left, int top, int right, int bottom) {
	return x >= left && x <= right && y <= top && y >= bottom;
}

bool is_inside_start_button(int x, int y) {
	printf("click x: %d\n", x);
	printf("click y: %d\n", y);
	return is_inside_rectangle(x, y, 500, 100, 600, 200);
}

void ve_nut_start() {
	outtextxy(500, 100, "START");
	rectangle(500, 100, 600, 200);
}

void VeHangDauKemTheoCap(int a[], int len, int index1, int index2) {
  int radius = default_radius;
  int duongkinh = default_duong_kinh;
  int x = start_point_x;
  int y = start_point_y + 200;
	int margin = default_margin;


  for (int i = 0; i < len; i++) {
		if (i > 0) {
			x = x + duongkinh + margin;
		}

    circle(x, y, radius);
		if (i == index1 || i == index2) {
			line(x, y + radius, x, y + duongkinh);
		}

		if (mat_nuoc != -1 && i == mat_nuoc) {
			line(x + radius + margin/2, y - radius, x + radius + margin/2, y + radius);
		}

	chon_mau(a, i);
    floodfill(x, y, drawing_color);
  }
}

// ve hang soi, khong ve 2 hat soi dang trong qua trinh di chuyen
void ve_hang_soi_dang_di_chuyen(int a[], int len, int left, int right, int after) {
  int radius = 30;
  int duongkinh = radius * 2;
  int x = start_point_x;
  int y = start_point_y + 200;
	int margin = default_margin;

  for (int i = 0; i < len; i++) {
		if (i > 0) {
			x = x + duongkinh + margin;
		}

  	if (i == left ||i == right) {
  		continue;
		}

		if (mat_nuoc != -1 && i == mat_nuoc) {
			line(x + radius + margin/2, y - radius, x + radius + margin/2, y + radius);
		}
	  
    circle(x, y, radius);

    chon_mau(a, i);
    floodfill(x, y, drawing_color);
  }
}

// ve hang soi sau khi da bien doi mang a
void VeHangSoi(int a[], int len) {
  int radius = default_radius;
  int duongkinh = default_duong_kinh;
  int x = start_point_x;
  int y = start_point_y + 200;
	int margin = default_margin;

  for (int i = 0; i < len; i++) {
		if (i > 0) {
			x = x + duongkinh + margin;
		}

		if (mat_nuoc != -1 && i == mat_nuoc) {
			line(x + radius + margin/2, y - radius, x + radius + margin/2, y + radius);
		}

    circle(x, y, radius);
	chon_mau(a, i);
    floodfill(x, y, drawing_color);
  }
}

void thuat_toan_theo_thu_tu(int thu_tu[]) {
	status = "Dang sap xep";
	int len = so_hat_soi;

  thu_tu_mau_sac[0] = thu_tu[0];
  thu_tu_mau_sac[1] = thu_tu[1];
  thu_tu_mau_sac[2] = thu_tu[2];

  // thay doi cac gia tri theo dung thu tu cua no
  for (int i = 0; i < len; i++) {
    // 1 do
    // 2 vang
    // 3 trang
    // 2 vang
    //
    // thu tu: vang, do, trang [2, 1, 3]
    //
    // [1, 2, 3, 2] => [2, 1, 3, 1] -- sap xep=> [1, 1, 2, 3] <=> [vang, vang, do, trang]
    if (hangsoi[i] == thu_tu[0]) {
      hangsoi[i] = 1;
    } else if (hangsoi[i] == thu_tu[1]) {
      hangsoi[i] = 2;
    } else {
      hangsoi[i] = 3;
    }
  }

  for (int i = len - 1; i >= 0; i--) {
		mat_nuoc = i;

    for (int j = 0; j < i; j++) {
			cleardevice();
			draw_status();
			ve_3_nut_bam();
      VeHangDauKemTheoCap(hangsoi, len, j, j + 1);
      delay(1000);
      cleardevice();

      if (hangsoi[j] > hangsoi[j + 1]) {
      	
      	di_chuyen_2_hat_soi(hangsoi, j, j+1);
      	
        int temp = hangsoi[j];
        hangsoi[j] = hangsoi[j + 1];
        hangsoi[j + 1] = temp;
      }

      cleardevice();
			draw_status();
			ve_3_nut_bam();
      VeHangSoi(hangsoi, len);
      delay(1000);
    }
  }
}






void chon_mau(int a[], int i) {
	int x = 100;
	int y = 100;
	int radius = 50;
	int khoang_cach = 10;
	
	int mau_nguyen_thuy = thu_tu_mau_sac[a[i] - 1];
	if (mau_nguyen_thuy == 1) {
      setfillstyle(SOLID_FILL, RED);
    } else if (mau_nguyen_thuy == 2) {
      setfillstyle(SOLID_FILL, YELLOW);
    } else {
      setfillstyle(SOLID_FILL, WHITE);
    }
}




bool kiem_tra_xy_nam_trong_hinh_tron(int x, int y, int tamX, int tamY, int radius) {
  return ((x >= tamX - radius) &&
    x <= tamX + radius &&
    y >= tamY - radius &&
    y <= tamY + radius);
}

bool kiem_tra_xy_trong_hinh_vuong(int left,int top,int right,int bottom,int x, int y)
{
	return(( x > left)
	&& (x < right)
	&& ( y > top)
	&& (y < bottom));
}


void ve_3_nut_bam() {
	draw_beautiful_start_button();
  int radius = 50;
  // ve hat dau mau do
  circle(100, 100, radius);
  setfillstyle(SOLID_FILL, RED);
  floodfill(100, 100, drawing_color);

  circle(210, 100, radius);
  setfillstyle(SOLID_FILL, YELLOW);
  floodfill(210, 100, drawing_color);

  circle(320, 100, radius);
  setfillstyle(SOLID_FILL, WHITE);
  floodfill(320, 100, drawing_color);
}

void VuaThemVuaVeHangSoi(int a[], int len) {
  int radius = 30;
  int duongkinh = radius * 2;
  int x = start_point_x;
  int y = start_point_y + 200;

  for (int i = 0; i < len; i++) {
    circle(x + i * (duongkinh + 10), y, radius);

    // 2 ham duoi to mau
    if (a[i] == 1) {
      setfillstyle(SOLID_FILL, RED);
    } else if (a[i] == 2) {
      setfillstyle(SOLID_FILL, YELLOW);
    } else {
      setfillstyle(SOLID_FILL, WHITE);
    }

    floodfill(x + i * (duongkinh + 10), y, drawing_color);
  }
}

void di_chuyen_2_hat_soi(int a[], int left, int right) {
	int x = start_point_x;
	int y = start_point_y + 200;
	int radius = 30;
	int khoang_cach = 10;
	
	int x_left = x + left * (radius * 2 + khoang_cach);
	int y_left = y;
	
	int x_right = x + right * (radius * 2 + khoang_cach);
	int y_right = y;
	
	int do_dai = x_right - x_left;
	int do_dai_moi_buoc = 10;
	int so_buoc = do_dai / do_dai_moi_buoc;
	
	int nhich_len = radius + 1;
	
	y_left = y_left + nhich_len;
	y_right = y_right - nhich_len;
	
	cleardevice();
	draw_status();
	ve_3_nut_bam();
	ve_hang_soi_dang_di_chuyen(a, so_hat_soi, left, right, so_hat_soi - 1);
	// hinh left nhich len, right di xuong
	circle(x_left, y_left, radius);
	chon_mau(a, left);
	floodfill(x_left, y_left, drawing_color);
	
	circle(x_right, y_right, radius);
	chon_mau(a, right);
	floodfill(x_right, y_right, drawing_color);
	delay(300);
	
	// left va right di ngang qua lai
	for (int i = 1; i <= so_buoc; i++) {
		cleardevice();
		draw_status();
		ve_3_nut_bam();
		ve_hang_soi_dang_di_chuyen(a, so_hat_soi, left, right, mat_nuoc);
		x_left = x_left + do_dai_moi_buoc;
		circle(x_left, y_left, radius);
		chon_mau(a, left);
		floodfill(x_left, y_left, drawing_color);
		
		x_right = x_right - do_dai_moi_buoc;
		circle(x_right, y_right, radius);
		chon_mau(a, right);
		floodfill(x_right, y_right, drawing_color);
		delay(300);
	}
	
	cleardevice();
	draw_status();
	ve_3_nut_bam();
	ve_hang_soi_dang_di_chuyen(a, so_hat_soi, left, right, mat_nuoc);
	// hinh left di xuong, right di len
	y_left = y_left - nhich_len;
	circle(x_left, y_left, radius);
	chon_mau(a, left);
	floodfill(x_left, y_left, drawing_color);
	
	circle(x_right, y_right, radius);
	y_right = y_right + nhich_len;
	chon_mau(a, right);
	floodfill(x_right, y_right, drawing_color);
}


bool should_render_for_add_stage = true;
void render_for_add_stage() {
	if (should_render_for_add_stage) {
		cleardevice();
		draw_status();
		ve_3_nut_bam();
		VuaThemVuaVeHangSoi(hangsoi, so_hat_soi);
		should_render_for_add_stage = false;
	}

	delay(10);
}

void add_stage() {
	button_text = "TIEP THEO";

  int x, y;
  while (TRUE) {
		render_for_add_stage();

    if (ismouseclick(WM_LBUTTONDOWN)) {
      getmouseclick(WM_LBUTTONDOWN, x, y);
      if (kiem_tra_xy_nam_trong_hinh_tron(x, y, 100, 100, 50)) {
      	PlaySound(TEXT("cach.wav"),NULL,SND_SYNC);
        status = "THEM HAT SOI: ban da click vien soi DO";
        hangsoi[vitri] = 1;
        vitri++;
        so_hat_soi++;
				should_render_for_add_stage = true;
      } else if (kiem_tra_xy_nam_trong_hinh_tron(x, y, 210, 100, 50)) {
      	PlaySound(TEXT("cach.wav"),NULL,SND_SYNC);
        status = "THEM HAT SOI: ban da click vien soi VANG";
        hangsoi[vitri] = 2;
        so_hat_soi++;
        vitri++;
				should_render_for_add_stage = true;
      } else if (kiem_tra_xy_nam_trong_hinh_tron(x, y, 320, 100, 50)) {
      	PlaySound(TEXT("cach.wav"),NULL,SND_SYNC);
        status = "THEM HAT SOI: ban da click vien soi TRANG";

        hangsoi[vitri] = 3;
        so_hat_soi++;
        vitri++;
				should_render_for_add_stage = true;
      } else { // neu click vao nut START
					if (kiem_tra_xy_trong_hinh_vuong(500,50,700,150,x,y) ){
						// kiem tra hang dau da chon duoc phan tu nao chua?
						if (so_hat_soi == 0)
						{
							status = "Vui long chon it nhat 1 hat soi de bat dau";
							should_render_for_add_stage = true;
							continue;
						}

						status = "Da click vao nut next. Ban muon xep theo thu tu nao?";
						should_render_for_add_stage = true;
						break;
					}
      }
    }
  }
}

bool should_render_for_order_stage = true;
void render_for_order_stage() {
	if (should_render_for_order_stage) {
		cleardevice();
		draw_status();
		ve_3_nut_bam();
		// ve hang dau
		VuaThemVuaVeHangSoi(hangsoi, so_hat_soi);
		should_render_for_order_stage = false;
	}

	delay(10);
}

void reset_game() {
	vitri = 0;
	so_hat_soi = 0;
	status = "Them hat soi bat ki ma ban muon";

	tong_da_chon = 0;
	dachondo = false;
	dachonvang = false;
	dachontrang = false;
}

void draw_thu_tu() {
	char tt[1000];
	char x[] = "THU TU:";
	strncat(tt, x, 7);
	

	for (int i = 0; i < tong_da_chon; i++) {
		switch(thu_tu_sap_xep[i])
		{
			case 1:
			{
				char content[] = "->DO";
				strncat(tt, content, strlen(content));
				break;
			}
			case 2:
			{
				char content[] = "->VANG";
				strncat(tt, content, strlen(content));
				break;
			}
			case 3:
			{
				char content[] = "->TRANG";
				strncat(tt, content, strlen(content));
				break;
			}
		}
	}

	status = tt;
	
}

void order_stage() {
	should_render_for_order_stage = true;
	button_text = "BAT DAU";
	int x, y;
	while (TRUE) {
		render_for_order_stage();

		if (tong_da_chon == 3) {
		//	outtext("nhan nut start");

			// wait for click start button. then exit (break)
			while(TRUE) {
				if (ismouseclick(WM_LBUTTONDOWN)) {
					getmouseclick(WM_LBUTTONDOWN, x, y);
					if (kiem_tra_xy_trong_hinh_vuong(500, 50,600,150,x,y) ){
						break;
					}
				}

				delay(10);
			}
			break;
		}
		
		if (ismouseclick(WM_LBUTTONDOWN)) {
			getmouseclick(WM_LBUTTONDOWN, x, y);
			if (kiem_tra_xy_nam_trong_hinh_tron(x, y, 100, 100, 50)) {
				if (!dachondo) {
					thu_tu_sap_xep[tong_da_chon] = 1;
					dachondo = true;
					tong_da_chon++;
					draw_thu_tu();
					should_render_for_order_stage = true;
				}
			} else if (kiem_tra_xy_nam_trong_hinh_tron(x, y, 210, 100, 50)) {
				if (!dachonvang) {
					thu_tu_sap_xep[tong_da_chon] = 2;
					dachonvang= true;
					tong_da_chon++;
					draw_thu_tu();
					should_render_for_order_stage = true;
				}
			} else if (kiem_tra_xy_nam_trong_hinh_tron(x, y, 320, 100, 50)) {
				if (!dachontrang) {
					thu_tu_sap_xep[tong_da_chon] = 3;
					dachontrang = true;
					tong_da_chon++;
					draw_thu_tu();
					should_render_for_order_stage = true;
				}
			}
		}
	}

	cleardevice();

	// tien hanh sap xep
	thuat_toan_theo_thu_tu(thu_tu_sap_xep);
}

int last_screen()
{   int x,y;
    setusercharsize(2,1,2,1);
	rectangle(200,50,400,150);
	char *chu_muon_viet1 = "QUAY LAI";
	int do_dai_cua_chu1 = textwidth(chu_muon_viet1);
	int do_cao_cua_chu1 = textheight(chu_muon_viet1);
	outtextxy(300-(do_dai_cua_chu1/2), 100 - (do_cao_cua_chu1/2), chu_muon_viet1);
	rectangle(600,50,800,150);
	char *chu_muon_viet2 = "KET THUC";
	int do_dai_cua_chu2 = textwidth(chu_muon_viet2);
	int do_cao_cua_chu2 = textheight(chu_muon_viet2);
	outtextxy(700-(do_dai_cua_chu2/2), 100 - (do_cao_cua_chu2/2), chu_muon_viet2);
	while(TRUE) {
		if (ismouseclick(WM_LBUTTONDOWN)) {
		getmouseclick(WM_LBUTTONDOWN, x, y);
	
	    if(kiem_tra_xy_trong_hinh_vuong(600, 50, 800, 150, x, y))
		{			
			return 1;
		}
		 if(kiem_tra_xy_trong_hinh_vuong(200, 50, 400, 150, x, y))
		 {
		    return 2;
		 }
	 }
	 
	 delay(100);
	}
}


void render_game() {
  initwindow(1000, 500, "ten window");
  setusercharsize(2, 1, 2, 1);

	setbkcolor(CYAN); // mau nen

	drawing_color = BLACK;
	setcolor(drawing_color); // drawing color == mau chu
	while(TRUE){
		switch(state) {
			case STATE_BEGIN:
				{
					reset_game();
					state = STATE_ADD;
				}
			case STATE_ADD:
				{
					add_stage();
					state = STATE_ORDER;
				}
			case STATE_ORDER:
				{
					order_stage();
					cleardevice();
					int flag = last_screen();
	                if (flag == 1) {
	   	            	closegraph();
	   	            	break;
	                }
	                if (flag == 2) {
	                	state = STATE_BEGIN;	
	                	continue;
					}
				}
			default:
				{
					break;
				}
		}
	}
	}



int main() {
	render_game();
  return 0;
  }

