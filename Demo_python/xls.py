#! coding=utf-8
import xlrd, xlwt


excel = xlrd.open_workbook(r"C:\Users\Administrator\Desktop\han.xlsx")
rtable = excel.sheets()[0]

merged_cells = rtable.merged_cells
n = rtable.cell(0, 0)
c = rtable.row(0)
print c

file = xlwt.Workbook()
wtable = file.add_sheet(rtable.name, cell_overwrite_ok=True)

#获取行数
nrows = rtable.nrows
ncols = rtable.ncols
for i in range(0, nrows):
    for j in range(0, ncols):
        wtable.write(i, j, rtable.cell(i, j).value)

file.save(r"C:\Users\Administrator\Desktop\han_new.xls")


def set_style(name, height, bold=False):
    style = xlwt.XFStyle() # 初始化样式

    font = xlwt.Font() # 为样式创建字体
    font.name = name # 'Times New Roman'
    font.bold = bold
    font.color_index = 4
    font.height = height
    font.outline

    alignment = xlwt.Alignment()
    alignment.vert = xlwt.Alignment.VERT_CENTER
    # borders= xlwt.Borders()
    # borders.left= 6
    # borders.right= 6
    # borders.top= 6
    # borders.bottom= 6
    style.font = font
    style.alignment = alignment
    # style.borders = borders
    return style

f = xlwt.Workbook() #创建工作簿
'''
创建第一个sheet:
sheet1
'''
sheet1 = f.add_sheet(u'sheet1',cell_overwrite_ok=True) #创建sheet
row0 = [u'业务',u'状态',u'北京',u'上海',u'广州',u'深圳',u'状态小计',u'合计']
column0 = [u'机票',u'船票',u'火车票',u'汽车票',u'其它']
status = [u'预订',u'出票',u'退票',u'业务小计']

#生成第一行
for i in range(0,len(row0)):
    sheet1.write(0, i, row0[i], set_style('Times New Roman',220,True))

#生成第一列和最后一列(合并4行)
i, j = 1, 0
while i < 4*len(column0) and j < len(column0):
    sheet1.write_merge(i, i + 3, 0, 0, column0[j], set_style('Arial',220,True)) #第一列
    sheet1.write_merge(i, i + 3, 7, 7) #最后一列"合计"
    i += 4
    j += 1
sheet1.write_merge(21,21,0,1,u'合计',set_style('Times New Roman',220,True))

#生成第二列
i = 0
while i < 4 * len(column0):
    for j in range(0, len(status)):
        sheet1.write(j+i+1, 1, status[j])
    i += 4

f.save(r'C:\Users\Administrator\Desktop\demo1.xlsx') #保存文件
