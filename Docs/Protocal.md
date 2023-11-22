# 消息协议设计文档
本文定义了用于 tcgr 项目的消息协议, 包括其格式, 语义, 以及使用方法.

在 ServerIO 类中使用, 将消息转换为服务器内部的数据结构, 并进行处理, 或是将处理结果转换为消息, 发送给客户端.

## 消息格式
消息格式为 JSON 格式, 由一个 JSON 对象组成, 包含以下字段:
1. 用户 UID
2. 消息类型
3. 消息内容

4. ...


### 消息类型
消息类型指明本次消息的目的. 分为

1. 用户登入/登出
2. 匹配请求
3. 对局开始/结束
4. 游戏操作
5. 操作结果反馈

### 示例

用户登入
```json
{
    "uid": 123456,
    "type": "login",
    "content": {
        "username": "tcgr",
        "password": "123456"
    }
}
```

匹配请求
```json
{
    "uid": 123456,
    "type": "match",
    "content": {
        "mode": "ranked"
    }
}
```
