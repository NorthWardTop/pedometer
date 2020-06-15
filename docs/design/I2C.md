<!-- ```flow
st=>start: 陪妹子进商场
op=>operation: 买买买
cond=>condition: 妹子开不开心?
e=>end: 出商场
st->op->cond
cond(yes)->e
cond(no)->op
``` -->


```flow
s=>start: 开始read
e=>end: 结束read
begin=>operation: beginTransmission()
end=>operation: endTransmission()
begin2=>operation: beginTransmission()
end2=>operation: endTransmission()
request=>operation: requestFrom(addr, len)
read_data=>operation: read_data()
cond=>condition: 读取完成



s->begin->request->end->begin2->read_data->cond
cond(yes)->end2->e
cond(no)->read_data



```