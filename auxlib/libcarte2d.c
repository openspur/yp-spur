// Copyright (c) 2010-2016 The YP-Spur Authors, except where otherwise indicated.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <cartesian2d.h>

CSptr CSroot_ptr;

/* 座標系の生成 */
CSptr CS_add(CSptr parent_cs, double x, double y, double theta)
{
  CSptr a_cs, p_cs;

  /* 新しい座標系を作成 */
  a_cs = (CSptr)malloc(sizeof(CoordinateSystem));
  if (!a_cs)
    return (CSptr)0; /* 作成失敗 */

  if (parent_cs == 0)
  { /* 座標系の神 */
    CSroot_ptr = a_cs;
    a_cs->parent = 0;
    a_cs->brother = 0;
    a_cs->child = 0;
    a_cs->x = 0;
    a_cs->y = 0;
    a_cs->theta = 0;
    a_cs->level = 0;
  }
  else
  { /* 誰かの子 */
    p_cs = parent_cs;

    if (!p_cs->child)
    { /* 長男 */
      p_cs->child = a_cs;
    }
    else
    { /* 既に兄がいる */
      p_cs = p_cs->child;
      while (p_cs->brother)
        p_cs = p_cs->brother;
      p_cs->brother = a_cs;
    }
    a_cs->parent = parent_cs;
    a_cs->brother = 0;
    a_cs->child = 0;
    a_cs->x = x;
    a_cs->y = y;
    a_cs->theta = theta;
    a_cs->level = parent_cs->level + 1;
  }

  return a_cs;
}

/* 座標系の削除（子・兄弟もろとも） */
/* 実装中途半端 */
int CS_delete(CSptr target_cs)
{
  if (target_cs->child)
  {
    CS_delete(target_cs->child);
  }

  if (target_cs->brother)
  {
    CS_delete(target_cs->brother);
  }

  if (target_cs)
  {
    free((void *)target_cs);
  }
  return 1;
}

/* 座標系の設定 */
int CS_set(CSptr target_cs, double x, double y, double theta)
{
  if (!target_cs)
    return 0;

  target_cs->x = x;
  target_cs->y = y;
  target_cs->theta = theta;

  return 1;
}

/* ある座標系上の座標を入力 */
int CS_set_on_CS(CSptr target_cs, CSptr on_cs, double x, double y, double theta)
{
  if (!target_cs || !on_cs)
    return 0;
  if (target_cs->parent)
    CS_recursive_trans(target_cs->parent, on_cs, &x, &y, &theta);
  return 1;
}

/* 子から見た親の位置を、親から見た自分の位置に変換する */
void CS_turn_base(double *x, double *y, double *theta)
{
  double xx, yy;

  xx = -(*x) * cos(-(*theta)) + (*y) * sin(-(*theta));
  yy = -(*x) * sin(-(*theta)) - (*y) * cos(-(*theta));
  *theta = -(*theta);
  *x = xx;
  *y = yy;
}

/*-----------------座標変換まわり-----------------*/
/* 目的の座標系へひとっとび？ */
/* 　　　　　　　　　　　　　 */
void CS_recursive_trans(CSptr target_cs, CSptr now_cs, double *x, double *y, double *theta)
{
  /* 座標系が有効か */
  if (!target_cs || !now_cs)
    return;
  // printf("now %d\n",now_cs->level);
  /* 同じ座標系か(case A) */
  if (target_cs == now_cs)
    return; /* 終了 */

  /* 下の座標系か */
  if (target_cs->level == now_cs->level)
  { /* 同じレベルにいるけど違う座標系 */
    // printf(".down from %d\n",now_cs->level);
    inv_trans_cs(now_cs, x, y, theta); /* 座標系をひとつ下る */
    CS_recursive_trans(target_cs->parent, now_cs->parent, x, y, theta);
    // printf(".up to %d\n" ,target_cs->level);
    trans_cs(target_cs, x, y, theta); /* 座標系をひとつ登る */
  }
  else if (target_cs->level > now_cs->level)
  {                                                             /* 現在位置の方が下 case C */
    CS_recursive_trans(target_cs->parent, now_cs, x, y, theta); /* ひとつ下る */
    // printf("up to %d\n",target_cs->level);
    trans_cs(target_cs, x, y, theta); /* 座標系をひとつ登る */
  }
  else
  { /* 現在位置の方が上 case D */
    // printf("down from %d\n" ,now_cs->level);
    inv_trans_cs(now_cs, x, y, theta);                          /* 座標系をひとつ下る */
    CS_recursive_trans(target_cs, now_cs->parent, x, y, theta); /* ひとつ下る */
  }

  return;
}

/* 座標系を一段下る（1段前の座標系での座標に変換する） */
void inv_trans_cs(CSptr target_cs, double *x, double *y, double *theta)
{
  double xx, yy;
  if (target_cs)
  {
    xx = *x * cos(target_cs->theta) - *y * sin(target_cs->theta) + target_cs->x;
    yy = *x * sin(target_cs->theta) + *y * cos(target_cs->theta) + target_cs->y;
    *x = xx;
    *y = yy;
    *theta += target_cs->theta;
  }
}

/* 座標系を一段あがる（一段後での座標系での座標に変換する） */
void trans_cs(CSptr target_cs, double *x, double *y, double *theta)
{
  double xx, yy;
  if (target_cs)
  {
    xx = (*x - target_cs->x) * cos(-target_cs->theta) - (*y - target_cs->y) * sin(-target_cs->theta);
    yy = (*x - target_cs->x) * sin(-target_cs->theta) + (*y - target_cs->y) * cos(-target_cs->theta);
    *x = xx;
    *y = yy;
    *theta -= target_cs->theta;
  }
}

/* 上にあがるだけ */
void trace_trans_cs(CSptr target_cs, double *x, double *y, double *theta)
{
  if (target_cs == CSroot_ptr)
    return;
  trace_trans_cs(target_cs->parent, x, y, theta);
  trans_cs(target_cs, x, y, theta);

  return;
}
