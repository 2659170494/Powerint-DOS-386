#include <stdio.h>
#include <string.h>
#include <syscall.h>
#include <math.h>
#include <locale.h>
#define LONG_MAX 0x7fffffff
#define LONG_MIN (-LONG_MAX - 1)
#define INT_MAX 0x7fffffff
#define INT_MIN (-LONG_MAX - 1)
// strcmp
int strcmp(const char* s1, const char* s2) {
  while (*s1 == *s2) {
    if (*s1 == '\0')
      return 0;
    s1++;
    s2++;
  }
  return *s1 - *s2;
}
// strcpy
char* strcpy(char* dest, const char* src) {
  char* tmp = dest;
  while ((*dest++ = *src++) != '\0')
    ;
  return tmp;
}
// strncpy
char* strncpy(char* dest, const char* src, size_t n) {
  char* tmp = dest;
  while (n-- > 0 && (*dest++ = *src++) != '\0')
    ;
  return tmp;
}
// strlen
size_t strlen(const char* s) {
  size_t len = 0;
  while (s[len])
    len++;
  return len;
}
// strcat
char* strcat(char* dest, const char* src) {
  char* tmp = dest;
  while (*dest)
    dest++;
  while ((*dest++ = *src++) != '\0')
    ;
  return tmp;
}
// strncat
char* strncat(char* dest, const char* src, size_t n) {
  char* tmp = dest;
  while (*dest)
    dest++;
  while (n-- > 0 && (*dest++ = *src++) != '\0')
    ;
  return tmp;
}
// memset
void* memset(void* s, int c, size_t n) {
  unsigned char* p = s;
  while (n-- > 0)
    *p++ = c;
  return s;
}
// strtol
long strtol(const char* nptr, char** endptr, int base) {
  long acc = 0;
  int c;
  unsigned long cutoff;
  int neg = 0, any, cutlim;

  /*
   * Skip white space and pick up leading +/- sign if any.
   * If base is 0, allow 0x for hex and 0 for octal, else
   * assume decimal; if base is already 16, allow 0x.
   */
  do {
    c = *nptr++;
  } while (isspace(c));
  if (c == '-') {
    neg = 1;
    c = *nptr++;
  } else if (c == '+')
    c = *nptr++;
  if ((base == 0 || base == 16) && c == '0' && (*nptr == 'x' || *nptr == 'X')) {
    c = nptr[1];
    nptr += 2;
    base = 16;
  } else if ((base == 0 || base == 2) && c == '0' &&
             (*nptr == 'b' || *nptr == 'B')) {
    c = nptr[1];
    nptr += 2;
    base = 2;
  } else if (base == 0)
    base = c == '0' ? 8 : 10;

  /*
   * Compute the cutoff value between legal numbers and illegal
   * numbers.  That is the largest legal value, divided by the
   * base.  An input number that is greater than this value, if
   * followed by a legal input character, is too big.  One that
   * is equal to this value may be valid or not; the limit
   * between valid and invalid numbers is then based on the last
   * digit.  For instance, if the range for longs is
   * [-2147483648..2147483647] and the input base is 10,
   * cutoff will be set to 214748364 and cutlim to either
   * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
   * a value > 214748364, or equal but the next digit is > 7 (or 8),
   * the number is too big, and we will return a range error.
   */
  cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
  cutlim = cutoff % (unsigned long)base;
  cutoff /= (unsigned long)base;
  for (acc = 0, any = 0;; c = *nptr++) {
    if (isdigit(c))
      c -= '0';
    else if (isalpha(c))
      c -= isupper(c) ? 'A' - 10 : 'a' - 10;
    else
      break;
    if (c >= base)
      break;
    if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
      any = -1;
    else {
      any = 1;
      acc *= base;
      acc += c;
    }
  }
  if (any < 0) {
    acc = neg ? LONG_MIN : LONG_MAX;
    // errno = ERANGE;
    print("panic: strtol: overflow\n");
  } else if (neg)
    acc = -acc;
  if (endptr != 0)
    *endptr = (char*)(any ? nptr : (char*)nptr - 1);
  return (acc);
}
// memcmp
int memcmp(const void* s1, const void* s2, size_t n) {
  const unsigned char *p1 = s1, *p2 = s2;
  while (n-- > 0) {
    if (*p1 != *p2)
      return *p1 - *p2;
    p1++, p2++;
  }
  return 0;
}
// memcpy
void* memcpy(void* s, const void* ct, size_t n) {
  if (NULL == s || NULL == ct || n <= 0)
    return NULL;
  while (n--)
    *(char*)s++ = *(char*)ct++;
  return s;
}
// isspace
int isspace(int c) {
  return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' ||
          c == '\v');
}
// isdigit
int isdigit(int c) {
  return (c >= '0' && c <= '9');
}
// isalpha
int isalpha(int c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
// isupper
int isupper(int c) {
  return (c >= 'A' && c <= 'Z');
}
// strncmp
int strncmp(const char* s1, const char* s2, size_t n) {
  const unsigned char *p1 = (const unsigned char*)s1,
                      *p2 = (const unsigned char*)s2;
  while (n-- > 0) {
    if (*p1 != *p2)
      return *p1 - *p2;
    if (*p1 == '\0')
      return 0;
    p1++, p2++;
  }
  return 0;
}
#define FLAG_ALTNT_FORM 0x01
#define FLAG_ALTNT_FORM_CH '#'

#define FLAG_ZERO_PAD 0x02
#define FLAG_ZERO_PAD_CH '0'

#define FLAG_LEFT_ADJUST 0x04
#define FLAG_LEFT_ADJUST_CH '-'

#define FLAG_SPACE_BEFORE_POS_NUM 0x08
#define FLAG_SPACE_BEFORE_POS_NUM_CH ' '

#define FLAG_SIGN 0x10
#define FLAG_SIGN_CH '+'

#define FLAG_LOWER 0x20

#define INT_TYPE_CHAR 0x1
#define INT_TYPE_SHORT 0x2
#define INT_TYPE_INT 0x4
#define INT_TYPE_LONG 0x8
#define INT_TYPE_LONG_LONG 0x10
#define INT_TYPE_MIN INT_TYPE_CHAR
#define INT_TYPE_MAX INT_TYPE_LONG_LONG

#define BUF_SIZE 4096

static char str_buf[BUF_SIZE];
static char num_str_buf[BUF_SIZE];

static char* int32_to_str_dec(int32_t num, int flag, int width) {
  int32_t num_tmp = num;
  char* p = num_str_buf;
  char* q = NULL;
  int len = 0;
  char* str_first = NULL;
  char* str_end = NULL;
  char ch = 0;

  memset(num_str_buf, 0, sizeof(num_str_buf));

  char dic[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

  if (num_tmp < 0) {
    *p++ = '-';
  } else if (flag & FLAG_SIGN) {
    *p++ = '+';
  } else {
    *p++ = ' ';
  }
  str_first = num_str_buf;

  if (num_tmp >= 0 && !(flag & FLAG_SIGN)) {
    str_first++;
  }

  if (num_tmp == 0) {
    *p++ = '0';
  } else {
    if (num_tmp < 0)
      num_tmp = -num_tmp;

    while (num_tmp) {
      *p++ = dic[num_tmp % 10];
      num_tmp /= 10;
    }
  }
  *p = '\0';

  str_end = p;
  len = str_end - str_first;

  q = num_str_buf + 1;
  p--;
  while (q < p) {
    ch = *q;
    *q = *p;
    *p = ch;
    p--;
    q++;
  }

  if (len < width) {
    p = str_end;

    if (flag & FLAG_LEFT_ADJUST) {
      for (int i = 0; i < width - len; i++)
        *p++ = ' ';
      *p = '\0';
      str_end = p;
    } else {
      while (p >= str_first) {
        *(p + width - len) = *p;
        p--;
      }

      if (flag & FLAG_ZERO_PAD) {
        for (int i = 0; i < width - len; i++) {
          num_str_buf[i + 1] = '0';
        }
      } else {
        for (int i = 0; i < width - len; i++)
          str_first[i] = ' ';
      }
    }
  }

  return str_first;
}

static char* int64_to_str_dec(int64_t num, int flag, int width) {
  return 0;
}

static char* uint32_to_str_hex(uint32_t num, int flag, int width) {
  uint32_t num_tmp = num;
  char* p = num_str_buf;
  char* q = NULL;
  int len = 0;
  char* str_first = NULL;
  char* str_end = NULL;
  char ch = 0;

  memset(num_str_buf, 0, sizeof(num_str_buf));

  char dic_lower[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                      '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  char dic_upper[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                      '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  char* dic = (flag & FLAG_LOWER) ? dic_lower : dic_upper;

  str_first = num_str_buf;

  *p++ = '0';
  *p++ = (flag & FLAG_LOWER) ? 'x' : 'X';

  if (!(flag & FLAG_ALTNT_FORM)) {
    str_first += 2;
  }

  do {
    *p++ = dic[num_tmp % 16];
    num_tmp /= 16;
  } while (num_tmp);
  *p = '\0';

  str_end = p;
  len = str_end - str_first;

  q = num_str_buf + 2;
  p--;
  while (q < p) {
    ch = *q;
    *q = *p;
    *p = ch;
    p--;
    q++;
  }

  if (len < width) {
    p = str_end;

    if (flag & FLAG_LEFT_ADJUST) {
      for (int i = 0; i < width - len; i++)
        *p++ = ' ';
      *p = '\0';
      str_end = p;
    } else {
      while (p >= str_first) {
        *(p + width - len) = *p;
        p--;
      }
      if (flag & FLAG_ALTNT_FORM)
        str_first[1] = (flag & FLAG_LOWER) ? 'x' : 'X';

      if (flag & FLAG_ZERO_PAD) {
        for (int i = 0; i < width - len; i++) {
          num_str_buf[i + 2] = '0';
        }
      } else {
        for (int i = 0; i < width - len; i++)
          str_first[i] = ' ';
      }
    }
  }

  if (num == 0 && flag & FLAG_ALTNT_FORM)
    str_first[1] = '0';

  return str_first;
}

static char* uint64_to_str_hex(uint64_t num, int flag, int width) {
  uint64_t num_tmp = num;
  char* p = num_str_buf;
  char* q = NULL;
  int len = 0;
  char* str_first = NULL;
  char* str_end = NULL;
  char ch = 0;

  memset(num_str_buf, 0, sizeof(num_str_buf));

  char dic_lower[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                      '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  char dic_upper[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                      '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  char* dic = (flag & FLAG_LOWER) ? dic_lower : dic_upper;

  str_first = num_str_buf;

  *p++ = '0';
  *p++ = (flag & FLAG_LOWER) ? 'x' : 'X';

  if (!(flag & FLAG_ALTNT_FORM)) {
    str_first += 2;
  }

  while (num_tmp) {
    *p++ = dic[num_tmp % 16];
    num_tmp /= 16;
  }
  *p = '\0';

  str_end = p;
  len = str_end - str_first;

  q = num_str_buf + 2;
  p--;
  while (q < p) {
    ch = *q;
    *q = *p;
    *p = ch;
    p--;
    q++;
  }

  if (len < width) {
    p = str_end;

    while (p >= str_first) {
      *(p + width - len) = *p;
      p--;
    }

    if (flag & FLAG_ZERO_PAD) {
      for (int i = 0; i < width - len; i++) {
        num_str_buf[i + 2] = '0';
      }
    } else {
      for (int i = 0; i < width - len; i++)
        str_first[i] = ' ';
    }
  }

  return str_first;
}

static char* uint32_to_str_oct(uint32_t num, int flag, int width) {
  uint32_t num_tmp = num;
  char* p = num_str_buf;
  char* q = NULL;
  int len = 0;
  char* str_first = NULL;
  char* str_end = NULL;
  char ch = 0;

  memset(num_str_buf, 0, sizeof(num_str_buf));

  char dic[] = {'0', '1', '2', '3', '4', '5', '6', '7'};

  str_first = num_str_buf;

  *p++ = '0';

  if (!(flag & FLAG_ALTNT_FORM)) {
    str_first += 1;
  }

  while (num_tmp) {
    *p++ = dic[num_tmp % 8];
    num_tmp /= 8;
  }
  *p = '\0';

  str_end = p;
  len = str_end - str_first;

  q = num_str_buf + 1;
  p--;
  while (q < p) {
    ch = *q;
    *q = *p;
    *p = ch;
    p--;
    q++;
  }

  if (len < width) {
    p = str_end;

    if (flag & FLAG_LEFT_ADJUST) {
      for (int i = 0; i < width - len; i++)
        *p++ = ' ';
      *p = '\0';
      str_end = p;
    } else {
      while (p >= str_first) {
        *(p + width - len) = *p;
        p--;
      }

      if (flag & FLAG_ZERO_PAD) {
        for (int i = 0; i < width - len; i++) {
          num_str_buf[i + 1] = '0';
        }
      } else {
        for (int i = 0; i < width - len; i++)
          str_first[i] = ' ';
      }
    }
  }

  return str_first;
}

static char* insert_str(char* buf, const char* str) {
  char* p = buf;

  while (*str) {
    *p++ = *str++;
  }

  return p;
}

int vsprintf(char* buf, const char* fmt, va_list args) {
  char* str = buf;
  int flag = 0;
  int int_type = INT_TYPE_INT;
  int tot_width = 0;
  int sub_width = 0;

  char* s = NULL;
  char ch = 0;
  int8_t num_8 = 0;
  uint8_t num_u8 = 0;
  int16_t num_16 = 0;
  uint16_t num_u16 = 0;
  int32_t num_32 = 0;
  uint32_t num_u32 = 0;
  int64_t num_64 = 0;
  uint64_t num_u64 = 0;

  for (const char* p = fmt; *p; p++) {
    if (*p != '%') {
      *str++ = *p;
      continue;
    }

    flag = 0;
    tot_width = 0;
    sub_width = 0;
    int_type = INT_TYPE_INT;

    p++;

    while (*p == FLAG_ALTNT_FORM_CH || *p == FLAG_ZERO_PAD_CH ||
           *p == FLAG_LEFT_ADJUST_CH || *p == FLAG_SPACE_BEFORE_POS_NUM_CH ||
           *p == FLAG_SIGN_CH) {
      if (*p == FLAG_ALTNT_FORM_CH) {
        flag |= FLAG_ALTNT_FORM;
      } else if (*p == FLAG_ZERO_PAD_CH) {
        flag |= FLAG_ZERO_PAD;
      } else if (*p == FLAG_LEFT_ADJUST_CH) {
        flag |= FLAG_LEFT_ADJUST;
        flag &= ~FLAG_ZERO_PAD;
      } else if (*p == FLAG_SPACE_BEFORE_POS_NUM_CH) {
        flag |= FLAG_SPACE_BEFORE_POS_NUM;
      } else if (*p == FLAG_SIGN_CH) {
        flag |= FLAG_SIGN;
      } else {
      }

      p++;
    }

    if (*p == '*') {
      tot_width = va_arg(args, int);
      if (tot_width < 0)
        tot_width = 0;
      p++;
    } else {
      while (isdigit(*p)) {
        tot_width = tot_width * 10 + *p - '0';
        p++;
      }
    }
    if (*p == '.') {
      if (*p == '*') {
        sub_width = va_arg(args, int);
        if (sub_width < 0)
          sub_width = 0;
        p++;
      } else {
        while (isdigit(*p)) {
          sub_width = sub_width * 10 + *p - '0';
          p++;
        }
      }
    }

  LOOP_switch:
    switch (*p) {
      case 'h':
        p++;
        if (int_type >= INT_TYPE_MIN) {
          int_type >>= 1;
          goto LOOP_switch;
        } else {
          *str++ = '%';
          break;
        }
      case 'l':
        p++;
        if (int_type <= INT_TYPE_MAX) {
          int_type <<= 1;
          goto LOOP_switch;
        } else {
          *str++ = '%';
          break;
        }
      case 's':
        s = va_arg(args, char*);
        str = insert_str(str, s);
        break;
      case 'c':
        ch = (char)(va_arg(args, int) & 0xFF);
        *str++ = ch;
        break;
      case 'd':
        switch (int_type) {
          case INT_TYPE_CHAR:
            num_8 = (int8_t)va_arg(args, int32_t);
            str = insert_str(str, int32_to_str_dec(num_8, flag, tot_width));
            break;
          case INT_TYPE_SHORT:
            num_16 = (int16_t)va_arg(args, int32_t);
            str = insert_str(str, int32_to_str_dec(num_16, flag, tot_width));
            break;
          case INT_TYPE_INT:
            num_32 = va_arg(args, int32_t);
            str = insert_str(str, int32_to_str_dec(num_32, flag, tot_width));
            break;
          case INT_TYPE_LONG:
            num_64 = va_arg(args, int64_t);
            str = insert_str(str, int64_to_str_dec(num_64, flag, tot_width));
            break;
          case INT_TYPE_LONG_LONG:
            num_64 = va_arg(args, int64_t);
            str = insert_str(str, int64_to_str_dec(num_64, flag, tot_width));
            break;
        }
        break;
      case 'x':
        flag |= FLAG_LOWER;
      case 'X':
        switch (int_type) {
          case INT_TYPE_CHAR:
            num_u8 = (uint8_t)va_arg(args, uint32_t);
            str = insert_str(str, uint32_to_str_hex(num_u8, flag, tot_width));
            break;
          case INT_TYPE_SHORT:
            num_u16 = (uint16_t)va_arg(args, uint32_t);
            str = insert_str(str, uint32_to_str_hex(num_u16, flag, tot_width));
            break;
          case INT_TYPE_INT:
            num_u32 = va_arg(args, uint32_t);
            str = insert_str(str, uint32_to_str_hex(num_u32, flag, tot_width));
            break;
          case INT_TYPE_LONG:
            num_u64 = va_arg(args, uint64_t);
            str = insert_str(str, uint64_to_str_hex(num_u64, flag, tot_width));
            break;
          case INT_TYPE_LONG_LONG:
            num_u64 = va_arg(args, uint64_t);
            str = insert_str(str, uint64_to_str_hex(num_u64, flag, tot_width));
            break;
        }
        break;
      case 'o':
        num_u32 = va_arg(args, uint32_t);
        str = insert_str(str, uint32_to_str_oct(num_u32, flag, tot_width));
        break;
      case '%':
        *str++ = '%';
        break;
      default:
        *str++ = '%';
        *str++ = *p;
        break;
    }
  }
  *str = '\0';

  return str - buf;
}
// sprintf
int sprintf(char* buf, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int len = vsprintf(buf, fmt, args);
  va_end(args);
  return len;
}
void strrev(char* s) {
  if (NULL == s)
    return;

  char* pBegin = s;
  char* pEnd = s + strlen(s) - 1;

  char pTemp;

  while (pBegin < pEnd) {
    pTemp = *pBegin;
    *pBegin = *pEnd;
    *pEnd = pTemp;

    ++pBegin, --pEnd;
  }
}
int printf(const char* format, ...) {
  int len;
  va_list ap;
  va_start(ap, format);
  char buf[1024];
  len = vsprintf(buf, format, ap);
  print(buf);
  va_end(ap);
  return len;
}
void F2S(double d, char* str, int l) {
  int n = (int)d;  //去掉小数点
  int b = 0;
  int i, j;
  double m = d;  //
  char* buf;
  buf = api_malloc(128);
  sprintf(str, "%d", n);
  i = strlen(str);
  str[i] = '.';  //小数点
  str[i + 1] = 0;
  while (d > 1.0) {
    d /= 10.0;
    b++;
  }
  for (i = 0; i < l; i++) {
    m *= 10;  //扩大
  }
  n = (int)m;  //放弃其他小数点
  sprintf(buf, "%d", n);
  for (i = b, j = strlen(str); i < strlen(buf); i++, j++) {
    str[j] = buf[i];
  }
  str[j] = 0;
  api_free(buf, 128);
  // 5.55 => 0.555 => <1 = true and b = 1
  // 1.56 0.156,15.6,156
}
char* strchr(const char* s, int c) {
  char* p = s;
  while (*p && *p != c) {
    p++;
  }
  if (*p == c) {
    return p;
  }
  return NULL;
}
char* strrchr(const char* s1, int ch) {
  char* s2;
  char* s3;
  s2 = strchr(s1, ch);
  while (s2 != NULL) {
    s3 = strchr(s2 + 1, ch);
    if (s3 != NULL) {
      s2 = s3;
    } else {
      return s2;
    }
  }
  return NULL;
}
void* memmove(void* dest, const void* src, int n) {
  /*因为char类型为1字节，所以将数据转化为char*
  进行操作。并不是因为操作的对象是字符串*/
  char* pdest = (char*)dest;
  const char* psrc = (const char*)src;
  if (pdest <= psrc && pdest >= psrc + n)  //正常情况下从前向后拷贝
  {
    while (n--) {
      *pdest = *psrc;
    }
  } else  //当出现内存覆盖时从后向前拷贝
  {
    while (n--) {
      *(pdest + n) = *(psrc + n);
    }
  }
  return dest;
}
void* malloc(int size) {
  void* p = api_malloc(size + sizeof(int));
  if (p == NULL)
    return NULL;
  *(int*)p = size;
  return (char*)p + sizeof(int);
}
void free(void* p) {
  if (p == NULL)
    return;
  int size = *(int*)((char*)p - sizeof(int));
  api_free((char*)p - sizeof(int), size + sizeof(int));
}
int _Znaj(uint32_t size) {
  //printk("_Znaj:%d\n", size);
  return malloc(size);
}
void _ZdaPv(void* ptr) {
  //printk("_ZdaPv:%08x\n", ptr);
  free(ptr);
}
//_ZdlPvj
void _ZdlPvj(void* ptr, uint32_t size) {
 // printk("_ZdlPvj %08x %d\n", ptr, size);
  free(ptr);
}
//_Znwj
void* _Znwj(uint32_t size) {
 // printk("_Znwj:%d\n", size);
  return malloc(size);
}
void *realloc(void *ptr, uint32_t size) {
  void *new = malloc(size);
  if (ptr) {
    memcpy(new, ptr, *(int *)((int)ptr - 4));
    free(ptr);
  }
  return new;
}
double scalbn(double x, int n)
{
	union {double f; uint64_t i;} u;
	double_t y = x;

	if (n > 1023) {
		y *= 0x1p1023;
		n -= 1023;
		if (n > 1023) {
			y *= 0x1p1023;
			n -= 1023;
			if (n > 1023)
				n = 1023;
		}
	} else if (n < -1022) {
		y *= 0x1p-1022;
		n += 1022;
		if (n < -1022) {
			y *= 0x1p-1022;
			n += 1022;
			if (n < -1022)
				n = -1022;
		}
	}
	u.i = (uint64_t)(0x3ff+n)<<52;
	x = y * u.f;
	return x;
}
static const double
pio2_hi = 1.57079632679489655800e+00, /* 0x3FF921FB, 0x54442D18 */
pio2_lo = 6.12323399573676603587e-17, /* 0x3C91A626, 0x33145C07 */
/* coefficients for R(x^2) */
pS0 =  1.66666666666666657415e-01, /* 0x3FC55555, 0x55555555 */
pS1 = -3.25565818622400915405e-01, /* 0xBFD4D612, 0x03EB6F7D */
pS2 =  2.01212532134862925881e-01, /* 0x3FC9C155, 0x0E884455 */
pS3 = -4.00555345006794114027e-02, /* 0xBFA48228, 0xB5688F3B */
pS4 =  7.91534994289814532176e-04, /* 0x3F49EFE0, 0x7501B288 */
pS5 =  3.47933107596021167570e-05, /* 0x3F023DE1, 0x0DFDF709 */
qS1 = -2.40339491173441421878e+00, /* 0xC0033A27, 0x1C8A2D4B */
qS2 =  2.02094576023350569471e+00, /* 0x40002AE5, 0x9C598AC8 */
qS3 = -6.88283971605453293030e-01, /* 0xBFE6066C, 0x1B8D0159 */
qS4 =  7.70381505559019352791e-02; /* 0x3FB3B8C5, 0xB12E9282 */

static double R(double z)
{
	double_t p, q;
	p = z*(pS0+z*(pS1+z*(pS2+z*(pS3+z*(pS4+z*pS5)))));
	q = 1.0+z*(qS1+z*(qS2+z*(qS3+z*qS4)));
	return p/q;
}

double asin(double x)
{
	double z,r,s;
	uint32_t hx,ix;

	GET_HIGH_WORD(hx, x);
	ix = hx & 0x7fffffff;
	/* |x| >= 1 or nan */
	if (ix >= 0x3ff00000) {
		uint32_t lx;
		GET_LOW_WORD(lx, x);
		if (((ix-0x3ff00000) | lx) == 0)
			/* asin(1) = +-pi/2 with inexact */
			return x*pio2_hi + 0x1p-120f;
		return 0/(x-x);
	}
	/* |x| < 0.5 */
	if (ix < 0x3fe00000) {
		/* if 0x1p-1022 <= |x| < 0x1p-26, avoid raising underflow */
		if (ix < 0x3e500000 && ix >= 0x00100000)
			return x;
		return x + x*R(x*x);
	}
	/* 1 > |x| >= 0.5 */
	z = (1 - fabs(x))*0.5;
	s = sqrt(z);
	r = R(z);
	if (ix >= 0x3fef3333) {  /* if |x| > 0.975 */
		x = pio2_hi-(2*(s+s*r)-pio2_lo);
	} else {
		double f,c;
		/* f+c = sqrt(z) */
		f = s;
		SET_LOW_WORD(f,0);
		c = (z-f*f)/(s+f);
		x = 0.5*pio2_hi - (2*s*r - (pio2_lo-2*c) - (0.5*pio2_hi-2*f));
	}
	if (hx >> 31)
		return -x;
	return x;
}
static const double atanhi[] = {
  4.63647609000806093515e-01, /* atan(0.5)hi 0x3FDDAC67, 0x0561BB4F */
  7.85398163397448278999e-01, /* atan(1.0)hi 0x3FE921FB, 0x54442D18 */
  9.82793723247329054082e-01, /* atan(1.5)hi 0x3FEF730B, 0xD281F69B */
  1.57079632679489655800e+00, /* atan(inf)hi 0x3FF921FB, 0x54442D18 */
};

static const double atanlo[] = {
  2.26987774529616870924e-17, /* atan(0.5)lo 0x3C7A2B7F, 0x222F65E2 */
  3.06161699786838301793e-17, /* atan(1.0)lo 0x3C81A626, 0x33145C07 */
  1.39033110312309984516e-17, /* atan(1.5)lo 0x3C700788, 0x7AF0CBBD */
  6.12323399573676603587e-17, /* atan(inf)lo 0x3C91A626, 0x33145C07 */
};

static const double aT[] = {
  3.33333333333329318027e-01, /* 0x3FD55555, 0x5555550D */
 -1.99999999998764832476e-01, /* 0xBFC99999, 0x9998EBC4 */
  1.42857142725034663711e-01, /* 0x3FC24924, 0x920083FF */
 -1.11111104054623557880e-01, /* 0xBFBC71C6, 0xFE231671 */
  9.09088713343650656196e-02, /* 0x3FB745CD, 0xC54C206E */
 -7.69187620504482999495e-02, /* 0xBFB3B0F2, 0xAF749A6D */
  6.66107313738753120669e-02, /* 0x3FB10D66, 0xA0D03D51 */
 -5.83357013379057348645e-02, /* 0xBFADDE2D, 0x52DEFD9A */
  4.97687799461593236017e-02, /* 0x3FA97B4B, 0x24760DEB */
 -3.65315727442169155270e-02, /* 0xBFA2B444, 0x2C6A6C2F */
  1.62858201153657823623e-02, /* 0x3F90AD3A, 0xE322DA11 */
};

double atan(double x)
{
	double_t w,s1,s2,z;
	uint32_t ix,sign;
	int id;

	GET_HIGH_WORD(ix, x);
	sign = ix >> 31;
	ix &= 0x7fffffff;
	if (ix >= 0x44100000) {   /* if |x| >= 2^66 */
		if (isnan(x))
			return x;
		z = atanhi[3] + 0x1p-120f;
		return sign ? -z : z;
	}
	if (ix < 0x3fdc0000) {    /* |x| < 0.4375 */
		if (ix < 0x3e400000) {  /* |x| < 2^-27 */
			if (ix < 0x00100000)
				/* raise underflow for subnormal x */
				FORCE_EVAL((float)x);
			return x;
		}
		id = -1;
	} else {
		x = fabs(x);
		if (ix < 0x3ff30000) {  /* |x| < 1.1875 */
			if (ix < 0x3fe60000) {  /*  7/16 <= |x| < 11/16 */
				id = 0;
				x = (2.0*x-1.0)/(2.0+x);
			} else {                /* 11/16 <= |x| < 19/16 */
				id = 1;
				x = (x-1.0)/(x+1.0);
			}
		} else {
			if (ix < 0x40038000) {  /* |x| < 2.4375 */
				id = 2;
				x = (x-1.5)/(1.0+1.5*x);
			} else {                /* 2.4375 <= |x| < 2^66 */
				id = 3;
				x = -1.0/x;
			}
		}
	}
	/* end of argument reduction */
	z = x*x;
	w = z*z;
	/* break sum from i=0 to 10 aT[i]z**(i+1) into odd and even poly */
	s1 = z*(aT[0]+w*(aT[2]+w*(aT[4]+w*(aT[6]+w*(aT[8]+w*aT[10])))));
	s2 = w*(aT[1]+w*(aT[3]+w*(aT[5]+w*(aT[7]+w*aT[9]))));
	if (id < 0)
		return x - x*(s1+s2);
	z = atanhi[id] - (x*(s1+s2) - atanlo[id] - x);
	return sign ? -z : z;
}
double scalbln(double x, long n)
{
	if (n > INT_MAX)
		n = INT_MAX;
	else if (n < INT_MIN)
		n = INT_MIN;
	return scalbn(x, n);
}
double ldexp(double x, int n)
{
	return scalbn(x, n);
}

static const double
pi     = 3.1415926535897931160E+00, /* 0x400921FB, 0x54442D18 */
pi_lo  = 1.2246467991473531772E-16; /* 0x3CA1A626, 0x33145C07 */

double atan2(double y, double x)
{
	double z;
	uint32_t m,lx,ly,ix,iy;

	if (isnan(x) || isnan(y))
		return x+y;
	EXTRACT_WORDS(ix, lx, x);
	EXTRACT_WORDS(iy, ly, y);
	if (((ix-0x3ff00000) | lx) == 0)  /* x = 1.0 */
		return atan(y);
	m = ((iy>>31)&1) | ((ix>>30)&2);  /* 2*sign(x)+sign(y) */
	ix = ix & 0x7fffffff;
	iy = iy & 0x7fffffff;

	/* when y = 0 */
	if ((iy|ly) == 0) {
		switch(m) {
		case 0:
		case 1: return y;   /* atan(+-0,+anything)=+-0 */
		case 2: return  pi; /* atan(+0,-anything) = pi */
		case 3: return -pi; /* atan(-0,-anything) =-pi */
		}
	}
	/* when x = 0 */
	if ((ix|lx) == 0)
		return m&1 ? -pi/2 : pi/2;
	/* when x is INF */
	if (ix == 0x7ff00000) {
		if (iy == 0x7ff00000) {
			switch(m) {
			case 0: return  pi/4;   /* atan(+INF,+INF) */
			case 1: return -pi/4;   /* atan(-INF,+INF) */
			case 2: return  3*pi/4; /* atan(+INF,-INF) */
			case 3: return -3*pi/4; /* atan(-INF,-INF) */
			}
		} else {
			switch(m) {
			case 0: return  0.0; /* atan(+...,+INF) */
			case 1: return -0.0; /* atan(-...,+INF) */
			case 2: return  pi;  /* atan(+...,-INF) */
			case 3: return -pi;  /* atan(-...,-INF) */
			}
		}
	}
	/* |y/x| > 0x1p64 */
	if (ix+(64<<20) < iy || iy == 0x7ff00000)
		return m&1 ? -pi/2 : pi/2;

	/* z = atan(|y/x|) without spurious underflow */
	if ((m&2) && iy+(64<<20) < ix)  /* |y/x| < 0x1p-64, x<0 */
		z = 0;
	else
		z = atan(fabs(y/x));
	switch (m) {
	case 0: return z;              /* atan(+,+) */
	case 1: return -z;             /* atan(-,+) */
	case 2: return pi - (z-pi_lo); /* atan(+,-) */
	default: /* case 3 */
		return (z-pi_lo) - pi; /* atan(-,-) */
	}
}
#define EPS (2.22044604925031308085e-16)
static const double_t toint = 1/EPS;

double floor(double x)
{
	union {double f; uint64_t i;} u = {x};
	int e = u.i >> 52 & 0x7ff;
	double_t y;

	if (e >= 0x3ff+52 || x == 0)
		return x;
	/* y = int(x) - x, where int(x) is an integer neighbor of x */
	if (u.i >> 63)
		y = x - toint + toint - x;
	else
		y = x + toint - toint - x;
	/* special case because of non-nearest rounding modes */
	if (e <= 0x3ff-1) {
		FORCE_EVAL(y);
		return u.i >> 63 ? -1 : 0;
	}
	if (y > 0)
		return x + y - 1;
	return x + y;
}
double ceil(double x)
{
	union {double f; uint64_t i;} u = {x};
	int e = u.i >> 52 & 0x7ff;
	double_t y;

	if (e >= 0x3ff+52 || x == 0)
		return x;
	/* y = int(x) - x, where int(x) is an integer neighbor of x */
	if (u.i >> 63)
		y = x - toint + toint - x;
	else
		y = x + toint - toint - x;
	/* special case because of non-nearest rounding modes */
	if (e <= 0x3ff-1) {
		FORCE_EVAL(y);
		return u.i >> 63 ? -0.0 : 1;
	}
	if (y < 0)
		return x + y + 1;
	return x + y;
}
static const double
bp[]   = {1.0, 1.5,},
dp_h[] = { 0.0, 5.84962487220764160156e-01,}, /* 0x3FE2B803, 0x40000000 */
dp_l[] = { 0.0, 1.35003920212974897128e-08,}, /* 0x3E4CFDEB, 0x43CFD006 */
two53  =  9007199254740992.0, /* 0x43400000, 0x00000000 */
huge   =  1.0e300,
tiny   =  1.0e-300,
/* poly coefs for (3/2)*(log(x)-2s-2/3*s**3 */
L1 =  5.99999999999994648725e-01, /* 0x3FE33333, 0x33333303 */
L2 =  4.28571428578550184252e-01, /* 0x3FDB6DB6, 0xDB6FABFF */
L3 =  3.33333329818377432918e-01, /* 0x3FD55555, 0x518F264D */
L4 =  2.72728123808534006489e-01, /* 0x3FD17460, 0xA91D4101 */
L5 =  2.30660745775561754067e-01, /* 0x3FCD864A, 0x93C9DB65 */
L6 =  2.06975017800338417784e-01, /* 0x3FCA7E28, 0x4A454EEF */
P1 =  1.66666666666666019037e-01, /* 0x3FC55555, 0x5555553E */
P2 = -2.77777777770155933842e-03, /* 0xBF66C16C, 0x16BEBD93 */
P3 =  6.61375632143793436117e-05, /* 0x3F11566A, 0xAF25DE2C */
P4 = -1.65339022054652515390e-06, /* 0xBEBBBD41, 0xC5D26BF1 */
P5 =  4.13813679705723846039e-08, /* 0x3E663769, 0x72BEA4D0 */
lg2     =  6.93147180559945286227e-01, /* 0x3FE62E42, 0xFEFA39EF */
lg2_h   =  6.93147182464599609375e-01, /* 0x3FE62E43, 0x00000000 */
lg2_l   = -1.90465429995776804525e-09, /* 0xBE205C61, 0x0CA86C39 */
ovt     =  8.0085662595372944372e-017, /* -(1024-log2(ovfl+.5ulp)) */
cp      =  9.61796693925975554329e-01, /* 0x3FEEC709, 0xDC3A03FD =2/(3ln2) */
cp_h    =  9.61796700954437255859e-01, /* 0x3FEEC709, 0xE0000000 =(float)cp */
cp_l    = -7.02846165095275826516e-09, /* 0xBE3E2FE0, 0x145B01F5 =tail of cp_h*/
ivln2   =  1.44269504088896338700e+00, /* 0x3FF71547, 0x652B82FE =1/ln2 */
ivln2_h =  1.44269502162933349609e+00, /* 0x3FF71547, 0x60000000 =24b 1/ln2*/
ivln2_l =  1.92596299112661746887e-08; /* 0x3E54AE0B, 0xF85DDF44 =1/ln2 tail*/

double pow(double x, double y)
{
	double z,ax,z_h,z_l,p_h,p_l;
	double y1,t1,t2,r,s,t,u,v,w;
	int32_t i,j,k,yisint,n;
	int32_t hx,hy,ix,iy;
	uint32_t lx,ly;

	EXTRACT_WORDS(hx, lx, x);
	EXTRACT_WORDS(hy, ly, y);
	ix = hx & 0x7fffffff;
	iy = hy & 0x7fffffff;

	/* x**0 = 1, even if x is NaN */
	if ((iy|ly) == 0)
		return 1.0;
	/* 1**y = 1, even if y is NaN */
	if (hx == 0x3ff00000 && lx == 0)
		return 1.0;
	/* NaN if either arg is NaN */
	if (ix > 0x7ff00000 || (ix == 0x7ff00000 && lx != 0) ||
	    iy > 0x7ff00000 || (iy == 0x7ff00000 && ly != 0))
		return x + y;

	/* determine if y is an odd int when x < 0
	 * yisint = 0       ... y is not an integer
	 * yisint = 1       ... y is an odd int
	 * yisint = 2       ... y is an even int
	 */
	yisint = 0;
	if (hx < 0) {
		if (iy >= 0x43400000)
			yisint = 2; /* even integer y */
		else if (iy >= 0x3ff00000) {
			k = (iy>>20) - 0x3ff;  /* exponent */
			if (k > 20) {
				uint32_t j = ly>>(52-k);
				if ((j<<(52-k)) == ly)
					yisint = 2 - (j&1);
			} else if (ly == 0) {
				uint32_t j = iy>>(20-k);
				if ((j<<(20-k)) == iy)
					yisint = 2 - (j&1);
			}
		}
	}

	/* special value of y */
	if (ly == 0) {
		if (iy == 0x7ff00000) {  /* y is +-inf */
			if (((ix-0x3ff00000)|lx) == 0)  /* (-1)**+-inf is 1 */
				return 1.0;
			else if (ix >= 0x3ff00000) /* (|x|>1)**+-inf = inf,0 */
				return hy >= 0 ? y : 0.0;
			else                       /* (|x|<1)**+-inf = 0,inf */
				return hy >= 0 ? 0.0 : -y;
		}
		if (iy == 0x3ff00000) {    /* y is +-1 */
			if (hy >= 0)
				return x;
			y = 1/x;
#if FLT_EVAL_METHOD!=0
			{
				union {double f; uint64_t i;} u = {y};
				uint64_t i = u.i & -1ULL/2;
				if (i>>52 == 0 && (i&(i-1)))
					FORCE_EVAL((float)y);
			}
#endif
			return y;
		}
		if (hy == 0x40000000)    /* y is 2 */
			return x*x;
		if (hy == 0x3fe00000) {  /* y is 0.5 */
			if (hx >= 0)     /* x >= +0 */
				return sqrt(x);
		}
	}

	ax = fabs(x);
	/* special value of x */
	if (lx == 0) {
		if (ix == 0x7ff00000 || ix == 0 || ix == 0x3ff00000) { /* x is +-0,+-inf,+-1 */
			z = ax;
			if (hy < 0)   /* z = (1/|x|) */
				z = 1.0/z;
			if (hx < 0) {
				if (((ix-0x3ff00000)|yisint) == 0) {
					z = (z-z)/(z-z); /* (-1)**non-int is NaN */
				} else if (yisint == 1)
					z = -z;          /* (x<0)**odd = -(|x|**odd) */
			}
			return z;
		}
	}

	s = 1.0; /* sign of result */
	if (hx < 0) {
		if (yisint == 0) /* (x<0)**(non-int) is NaN */
			return (x-x)/(x-x);
		if (yisint == 1) /* (x<0)**(odd int) */
			s = -1.0;
	}

	/* |y| is huge */
	if (iy > 0x41e00000) { /* if |y| > 2**31 */
		if (iy > 0x43f00000) {  /* if |y| > 2**64, must o/uflow */
			if (ix <= 0x3fefffff)
				return hy < 0 ? huge*huge : tiny*tiny;
			if (ix >= 0x3ff00000)
				return hy > 0 ? huge*huge : tiny*tiny;
		}
		/* over/underflow if x is not close to one */
		if (ix < 0x3fefffff)
			return hy < 0 ? s*huge*huge : s*tiny*tiny;
		if (ix > 0x3ff00000)
			return hy > 0 ? s*huge*huge : s*tiny*tiny;
		/* now |1-x| is tiny <= 2**-20, suffice to compute
		   log(x) by x-x^2/2+x^3/3-x^4/4 */
		t = ax - 1.0;       /* t has 20 trailing zeros */
		w = (t*t)*(0.5 - t*(0.3333333333333333333333-t*0.25));
		u = ivln2_h*t;      /* ivln2_h has 21 sig. bits */
		v = t*ivln2_l - w*ivln2;
		t1 = u + v;
		SET_LOW_WORD(t1, 0);
		t2 = v - (t1-u);
	} else {
		double ss,s2,s_h,s_l,t_h,t_l;
		n = 0;
		/* take care subnormal number */
		if (ix < 0x00100000) {
			ax *= two53;
			n -= 53;
			GET_HIGH_WORD(ix,ax);
		}
		n += ((ix)>>20) - 0x3ff;
		j = ix & 0x000fffff;
		/* determine interval */
		ix = j | 0x3ff00000;   /* normalize ix */
		if (j <= 0x3988E)      /* |x|<sqrt(3/2) */
			k = 0;
		else if (j < 0xBB67A)  /* |x|<sqrt(3)   */
			k = 1;
		else {
			k = 0;
			n += 1;
			ix -= 0x00100000;
		}
		SET_HIGH_WORD(ax, ix);

		/* compute ss = s_h+s_l = (x-1)/(x+1) or (x-1.5)/(x+1.5) */
		u = ax - bp[k];        /* bp[0]=1.0, bp[1]=1.5 */
		v = 1.0/(ax+bp[k]);
		ss = u*v;
		s_h = ss;
		SET_LOW_WORD(s_h, 0);
		/* t_h=ax+bp[k] High */
		t_h = 0.0;
		SET_HIGH_WORD(t_h, ((ix>>1)|0x20000000) + 0x00080000 + (k<<18));
		t_l = ax - (t_h-bp[k]);
		s_l = v*((u-s_h*t_h)-s_h*t_l);
		/* compute log(ax) */
		s2 = ss*ss;
		r = s2*s2*(L1+s2*(L2+s2*(L3+s2*(L4+s2*(L5+s2*L6)))));
		r += s_l*(s_h+ss);
		s2 = s_h*s_h;
		t_h = 3.0 + s2 + r;
		SET_LOW_WORD(t_h, 0);
		t_l = r - ((t_h-3.0)-s2);
		/* u+v = ss*(1+...) */
		u = s_h*t_h;
		v = s_l*t_h + t_l*ss;
		/* 2/(3log2)*(ss+...) */
		p_h = u + v;
		SET_LOW_WORD(p_h, 0);
		p_l = v - (p_h-u);
		z_h = cp_h*p_h;        /* cp_h+cp_l = 2/(3*log2) */
		z_l = cp_l*p_h+p_l*cp + dp_l[k];
		/* log2(ax) = (ss+..)*2/(3*log2) = n + dp_h + z_h + z_l */
		t = (double)n;
		t1 = ((z_h + z_l) + dp_h[k]) + t;
		SET_LOW_WORD(t1, 0);
		t2 = z_l - (((t1 - t) - dp_h[k]) - z_h);
	}

	/* split up y into y1+y2 and compute (y1+y2)*(t1+t2) */
	y1 = y;
	SET_LOW_WORD(y1, 0);
	p_l = (y-y1)*t1 + y*t2;
	p_h = y1*t1;
	z = p_l + p_h;
	EXTRACT_WORDS(j, i, z);
	if (j >= 0x40900000) {                      /* z >= 1024 */
		if (((j-0x40900000)|i) != 0)        /* if z > 1024 */
			return s*huge*huge;         /* overflow */
		if (p_l + ovt > z - p_h)
			return s*huge*huge;         /* overflow */
	} else if ((j&0x7fffffff) >= 0x4090cc00) {  /* z <= -1075 */
		if (((j-0xc090cc00)|i) != 0)        /* z < -1075 */
			return s*tiny*tiny;         /* underflow */
		if (p_l <= z - p_h)
			return s*tiny*tiny;         /* underflow */
	}
	/*
	 * compute 2**(p_h+p_l)
	 */
	i = j & 0x7fffffff;
	k = (i>>20) - 0x3ff;
	n = 0;
	if (i > 0x3fe00000) {  /* if |z| > 0.5, set n = [z+0.5] */
		n = j + (0x00100000>>(k+1));
		k = ((n&0x7fffffff)>>20) - 0x3ff;  /* new k for n */
		t = 0.0;
		SET_HIGH_WORD(t, n & ~(0x000fffff>>k));
		n = ((n&0x000fffff)|0x00100000)>>(20-k);
		if (j < 0)
			n = -n;
		p_h -= t;
	}
	t = p_l + p_h;
	SET_LOW_WORD(t, 0);
	u = t*lg2_h;
	v = (p_l-(t-p_h))*lg2 + t*lg2_l;
	z = u + v;
	w = v - (z-u);
	t = z*z;
	t1 = z - t*(P1+t*(P2+t*(P3+t*(P4+t*P5))));
	r = (z*t1)/(t1-2.0) - (w + z*w);
	z = 1.0 - (r-z);
	GET_HIGH_WORD(j, z);
	j += n<<20;
	if ((j>>20) <= 0)  /* subnormal output */
		z = scalbn(z,n);
	else
		SET_HIGH_WORD(z, j);
	return s*z;
}
static const double
half[2] = {0.5,-0.5},
ln2hi = 6.93147180369123816490e-01, /* 0x3fe62e42, 0xfee00000 */
ln2lo = 1.90821492927058770002e-10, /* 0x3dea39ef, 0x35793c76 */
invln2 = 1.44269504088896338700e+00, /* 0x3ff71547, 0x652b82fe */
P11   =  1.66666666666666019037e-01, /* 0x3FC55555, 0x5555553E */
P21   = -2.77777777770155933842e-03, /* 0xBF66C16C, 0x16BEBD93 */
P31   =  6.61375632143793436117e-05, /* 0x3F11566A, 0xAF25DE2C */
P41   = -1.65339022054652515390e-06, /* 0xBEBBBD41, 0xC5D26BF1 */
P51   =  4.13813679705723846039e-08; /* 0x3E663769, 0x72BEA4D0 */

double exp(double x)
{
	double_t hi, lo, c, xx, y;
	int k, sign;
	uint32_t hx;

	GET_HIGH_WORD(hx, x);
	sign = hx>>31;
	hx &= 0x7fffffff;  /* high word of |x| */

	/* special cases */
	if (hx >= 0x4086232b) {  /* if |x| >= 708.39... */
		if (isnan(x))
			return x;
		if (x > 709.782712893383973096) {
			/* overflow if x!=inf */
			x *= 0x1p1023;
			return x;
		}
		if (x < -708.39641853226410622) {
			/* underflow if x!=-inf */
			FORCE_EVAL((float)(-0x1p-149/x));
			if (x < -745.13321910194110842)
				return 0;
		}
	}

	/* argument reduction */
	if (hx > 0x3fd62e42) {  /* if |x| > 0.5 ln2 */
		if (hx >= 0x3ff0a2b2)  /* if |x| >= 1.5 ln2 */
			k = (int)(invln2*x + half[sign]);
		else
			k = 1 - sign - sign;
		hi = x - k*ln2hi;  /* k*ln2hi is exact here */
		lo = k*ln2lo;
		x = hi - lo;
	} else if (hx > 0x3e300000)  {  /* if |x| > 2**-28 */
		k = 0;
		hi = x;
		lo = 0;
	} else {
		/* inexact if x!=0 */
		FORCE_EVAL(0x1p1023 + x);
		return 1 + x;
	}

	/* x is now in primary range */
	xx = x*x;
	c = x - xx*(P11+xx*(P21+xx*(P31+xx*(P41+xx*P51))));
	y = 1 + (x*c/(2-c) - lo + hi);
	if (k == 0)
		return y;
	return scalbn(y, k);
}
double fmod(double x, double y)
{
	union {double f; uint64_t i;} ux = {x}, uy = {y};
	int ex = ux.i>>52 & 0x7ff;
	int ey = uy.i>>52 & 0x7ff;
	int sx = ux.i>>63;
	uint64_t i;

	/* in the followings uxi should be ux.i, but then gcc wrongly adds */
	/* float load/store to inner loops ruining performance and code size */
	uint64_t uxi = ux.i;

	if (uy.i<<1 == 0 || isnan(y) || ex == 0x7ff)
		return (x*y)/(x*y);
	if (uxi<<1 <= uy.i<<1) {
		if (uxi<<1 == uy.i<<1)
			return 0*x;
		return x;
	}

	/* normalize x and y */
	if (!ex) {
		for (i = uxi<<12; i>>63 == 0; ex--, i <<= 1);
		uxi <<= -ex + 1;
	} else {
		uxi &= -1ULL >> 12;
		uxi |= 1ULL << 52;
	}
	if (!ey) {
		for (i = uy.i<<12; i>>63 == 0; ey--, i <<= 1);
		uy.i <<= -ey + 1;
	} else {
		uy.i &= -1ULL >> 12;
		uy.i |= 1ULL << 52;
	}

	/* x mod y */
	for (; ex > ey; ex--) {
		i = uxi - uy.i;
		if (i >> 63 == 0) {
			if (i == 0)
				return 0*x;
			uxi = i;
		}
		uxi <<= 1;
	}
	i = uxi - uy.i;
	if (i >> 63 == 0) {
		if (i == 0)
			return 0*x;
		uxi = i;
	}
	for (; uxi>>52 == 0; uxi <<= 1, ex--);

	/* scale result */
	if (ex > 0) {
		uxi -= 1ULL << 52;
		uxi |= (uint64_t)ex << 52;
	} else {
		uxi >>= -ex + 1;
	}
	uxi |= (uint64_t)sx << 63;
	ux.i = uxi;
	return ux.f;
}
uint64_t __udivmoddi4(uint64_t num, uint64_t den, uint64_t * rem_p)
{
    uint64_t quot = 0, qbit = 1;

    if (den == 0) {
	__asm__ __volatile__ ("int $0");
	return 0;		/* If trap returns... */
    }

    /* Left-justify denominator and count shift */
    while ((int64_t) den >= 0) {
	den <<= 1;
	qbit <<= 1;
    }

    while (qbit) {
	if (den <= num) {
	    num -= den;
	    quot += qbit;
	}
	den >>= 1;
	qbit >>= 1;
    }

    if (rem_p)
	*rem_p = num;

    return quot;
}
typedef unsigned int UQItype	__attribute__ ((mode (QI)));
typedef          int SItype	__attribute__ ((mode (SI)));
typedef unsigned int USItype	__attribute__ ((mode (SI)));
typedef          int DItype	__attribute__ ((mode (DI)));
typedef unsigned int UDItype	__attribute__ ((mode (DI)));
#define Wtype SItype
#define HWtype SItype
#define DWtype DItype
#define UWtype USItype
#define UHWtype USItype
#define UDWtype UDItype
#define W_TYPE_SIZE 32
DWtype
__divdi3 (DWtype u, DWtype v)
{
  Wtype c = 0;
  DWtype w;

  if (u < 0)
    {
      c = ~c;
      u = -u;
    }
  if (v < 0)
    {
      c = ~c;
      v = -v;
    }
  w = __udivmoddi4 (u, v, NULL);
  if (c)
    w = -w;
  return w;
}
//strong_alias (__divdi3, __divdi3_internal)

DWtype
__moddi3 (DWtype u, DWtype v)
{
  Wtype c = 0;
  DWtype w;

  if (u < 0)
    {
      c = ~c;
      u = -u;
    }
  if (v < 0)
    v = -v;
  __udivmoddi4 (u, v, (UDWtype *) &w);
  if (c)
    w = -w;
  return w;
}
//strong_alias (__moddi3, __moddi3_internal)

UDWtype
__udivdi3 (UDWtype u, UDWtype v)
{
  return __udivmoddi4 (u, v, NULL);
}
//strong_alias (__udivdi3, __udivdi3_internal)

UDWtype
__umoddi3 (UDWtype u, UDWtype v)
{
  UDWtype w;

  __udivmoddi4 (u, v, &w);
  return w;
}
static const double
ivln10hi  = 4.34294481878168880939e-01, /* 0x3fdbcb7b, 0x15200000 */
ivln10lo  = 2.50829467116452752298e-11, /* 0x3dbb9438, 0xca9aadd5 */
log10_2hi = 3.01029995663611771306e-01, /* 0x3FD34413, 0x509F6000 */
log10_2lo = 3.69423907715893078616e-13, /* 0x3D59FEF3, 0x11F12B36 */
Lg1 = 6.666666666666735130e-01,  /* 3FE55555 55555593 */
Lg2 = 3.999999999940941908e-01,  /* 3FD99999 9997FA04 */
Lg3 = 2.857142874366239149e-01,  /* 3FD24924 94229359 */
Lg4 = 2.222219843214978396e-01,  /* 3FCC71C5 1D8E78AF */
Lg5 = 1.818357216161805012e-01,  /* 3FC74664 96CB03DE */
Lg6 = 1.531383769920937332e-01,  /* 3FC39A09 D078C69F */
Lg7 = 1.479819860511658591e-01;  /* 3FC2F112 DF3E5244 */

double log10(double x)
{
	union {double f; uint64_t i;} u = {x};
	double_t hfsq,f,s,z,R,w,t1,t2,dk,y,hi,lo,val_hi,val_lo;
	uint32_t hx;
	int k;

	hx = u.i>>32;
	k = 0;
	if (hx < 0x00100000 || hx>>31) {
		if (u.i<<1 == 0)
			return -1/(x*x);  /* log(+-0)=-inf */
		if (hx>>31)
			return (x-x)/0.0; /* log(-#) = NaN */
		/* subnormal number, scale x up */
		k -= 54;
		x *= 0x1p54;
		u.f = x;
		hx = u.i>>32;
	} else if (hx >= 0x7ff00000) {
		return x;
	} else if (hx == 0x3ff00000 && u.i<<32 == 0)
		return 0;

	/* reduce x into [sqrt(2)/2, sqrt(2)] */
	hx += 0x3ff00000 - 0x3fe6a09e;
	k += (int)(hx>>20) - 0x3ff;
	hx = (hx&0x000fffff) + 0x3fe6a09e;
	u.i = (uint64_t)hx<<32 | (u.i&0xffffffff);
	x = u.f;

	f = x - 1.0;
	hfsq = 0.5*f*f;
	s = f/(2.0+f);
	z = s*s;
	w = z*z;
	t1 = w*(Lg2+w*(Lg4+w*Lg6));
	t2 = z*(Lg1+w*(Lg3+w*(Lg5+w*Lg7)));
	R = t2 + t1;

	/* See log2.c for details. */
	/* hi+lo = f - hfsq + s*(hfsq+R) ~ log(1+f) */
	hi = f - hfsq;
	u.f = hi;
	u.i &= (uint64_t)-1<<32;
	hi = u.f;
	lo = f - hi - hfsq + s*(hfsq+R);

	/* val_hi+val_lo ~ log10(1+f) + k*log10(2) */
	val_hi = hi*ivln10hi;
	dk = k;
	y = dk*log10_2hi;
	val_lo = dk*log10_2lo + (lo+hi)*ivln10lo + lo*ivln10hi;

	/*
	 * Extra precision in for adding y is not strictly needed
	 * since there is no very large cancellation near x = sqrt(2) or
	 * x = 1/sqrt(2), but we do it anyway since it costs little on CPUs
	 * with some parallelism and it reduces the error for many args.
	 */
	w = y + val_hi;
	val_lo += (y - w) + val_hi;
	val_hi = w;

	return val_lo + val_hi;
}
void *memchr(const void * s, int c, size_t n)
{
	const unsigned char *p = s;

	while (n-- != 0)
	{
        if ((unsigned char)c == *p++)
        {
			return (void *)(p - 1);
		}
	}

	return NULL;
}
/*
 * Convert string to double
 */
double strtod(const char * nptr, char ** endptr)
{
	double number;
	int exponent;
	int negative;
	char * p = (char *)nptr;
	double p10;
	int n;
	int num_digits;
	int num_decimals;

	/* Skip leading whitespace */
	while(isspace(*p))
		p++;

	/* Handle optional sign */
	negative = 0;
	switch(*p)
	{
	case '-':
		negative = 1;
		p++;
		break;
	case '+':
		p++;
		break;
	}

	number = 0.;
	exponent = 0;
	num_digits = 0;
	num_decimals = 0;

	/* Process string of digits */
	while(isdigit(*p))
	{
		number = number * 10. + (*p - '0');
		p++;
		num_digits++;
	}

	/* Process decimal part */
	if(*p == '.')
	{
		p++;

		while(isdigit(*p))
		{
			number = number * 10. + (*p - '0');
			p++;
			num_digits++;
			num_decimals++;
		}

		exponent -= num_decimals;
	}

	if(num_digits == 0)
	{
		return 0.0;
	}

	/* Correct for sign */
	if(negative)
		number = -number;

	/* Process an exponent string */
	if(*p == 'e' || *p == 'E')
	{
		/* Handle optional sign */
		negative = 0;
		switch(*++p)
		{
		case '-':
			negative = 1;
			p++;
			break;
		case '+':
			p++;
			break;
		}

		/* Process string of digits */
		n = 0;
		while(isdigit(*p))
		{
			n = n * 10 + (*p - '0');
			p++;
		}

		if(negative)
			exponent -= n;
		else
			exponent += n;
	}

	if(exponent < -307 || exponent > 308)
	{
		return 0.0;
	}

	/* Scale the result */
	p10 = 10.;
	n = exponent;
	if(n < 0)
		n = -n;
	while(n)
	{
		if(n & 1)
		{
			if (exponent < 0)
				number /= p10;
			else
				number *= p10;
		}
		n >>= 1;
		p10 *= p10;
	}

	if(endptr)
		*endptr = p;

	return number;
}
void abort(void)
{
    exit();
}

const char* strstr(const char* p1, const char* p2)
{
	char* s1 = (char*)p1;
	char* s2 = (char*)p2;
	char* cur = (char*)p1;
	if (*p2 == '\0')
	{
		return p1;
	}
	while (*cur)
	{
		s1 = cur;
		s2 = (char*)p2;
		while ((*s1 != '\0') && (*s2 != '\0') && (*s1 == *s2))
		{
			s1++;
			s2++;
		}
		if (*s2 == '\0')
		{
			return cur;
		}
		cur++;
	}
	return NULL;
}
int toupper(int c)
{
	if ((c >= 'a') && (c <= 'z'))
		return c + ('A' - 'a');
	return c;
}
int tolower(int c)
{
	if ((c >= 'A') && (c <= 'Z'))
		return c + ('a' - 'A');
	return c;
}
int isalnum(int ch)
{
	return (unsigned int)((ch | 0x20) - 'a') < 26u ||
		(unsigned int)( ch - '0') < 10u;
}
int iscntrl(int ch)
{
    return (unsigned int)ch < 32u  ||  ch == 127;
}//判断字符c是否为控制字符。当c在0x00-0x1F之间或等于0x7F(DEL)时，返回非零值，否则返回零。
int islower (int ch)
{
    return (unsigned int) (ch - 'a') < 26u;
}//判断字符c是否为小写英文字母
int isxdigit (int c)
{
	if (('0' <= c && c <= '9') || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F')) {
		return 1;
	}
	return 0;
}
int isgraph(int ch)
{
    return (unsigned int)(ch - '!') < 127u - '!';
}   //判断字符c是否为除空格外的可打印字符。可打印字符（0x21-0x7e）。
int isprint( int ch )
{
    return (unsigned int)(ch - ' ') < 127u - ' ';
}//判断字符c是否为可打印字符（含空格）。当c为可打印字符（0x20-0x7e）时，返回非零值，否则返回零。
int ispunct(int ch)
{
    return isprint(ch)  &&  !isalnum (ch)  &&  !isspace (ch);
}//判断字符c是否为标点符号。标点符号指那些既不是字母数字，也不是空格的可打印字符。
char *
strpbrk (s, accept)
     register const char *s;
     register const char *accept;
{
  while (*s != '\0')
    {
      const char *a = accept;
      while (*a != '\0')
	if (*a++ == *s)
	  return (char *) s;
      ++s;
    }

  return 0;
}
size_t strspn(const char *s, const char *accept)
{
    const char *p = s;
    const char *a;
    size_t count = 0;

    for (; *p != '\0'; ++p) {
        for (a = accept; *a != '\0'; ++a) {
            if (*p == *a)
                break;
        }
        if (*a == '\0')
            return count;
        ++count;
    }
    return count;
}
int strcoll(const char *str1, const char *str2)
{
    return strcmp(str1, str2);
}
double frexp(double x, int *e)
{
	union { double d; uint64_t i; } y = { x };
	int ee = y.i>>52 & 0x7ff;

	if (!ee) {
		if (x) {
			x = frexp(x*0x1p64, e);
			*e -= 64;
		} else *e = 0;
		return x;
	} else if (ee == 0x7ff) {
		return x;
	}

	*e = ee - 0x3fe;
	y.i &= 0x800fffffffffffffull;
	y.i |= 0x3fe0000000000000ull;
	return y.d;
}
static const struct lconv posix_lconv = {
	.decimal_point = ".",
	.thousands_sep = "",
	.grouping = "",
	.int_curr_symbol = "",
	.currency_symbol = "",
	.mon_decimal_point = "",
	.mon_thousands_sep = "",
	.mon_grouping = "",
	.positive_sign = "",
	.negative_sign = "",
	.int_frac_digits = -1,
	.frac_digits = -1,
	.p_cs_precedes = -1,
	.p_sep_by_space = -1,
	.n_cs_precedes = -1,
	.n_sep_by_space = -1,
	.p_sign_posn = -1,
	.n_sign_posn = -1,
	.int_p_cs_precedes = -1,
	.int_p_sep_by_space = -1,
	.int_n_cs_precedes = -1,
	.int_n_sep_by_space = -1,
	.int_p_sign_posn = -1,
	.int_n_sign_posn = -1,
};

struct lconv * localeconv(void)
{
	return (struct lconv *)&posix_lconv;
}