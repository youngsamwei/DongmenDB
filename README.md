# DongmenDB

DongmenDB 是一个用于教学的关系数据库系统

# 使用须知

如果你是正在使用本仓库进行实验的同学，请确保知悉并遵循以下几点：

- 在你开始实验或者提问之前，请确保已经阅读完该仓库的`README`文件以及`doc` 文件夹下的指导性文件
- 本仓库旨在加深学生对数据库相关知识的理解与提高动手能力，请不要在未理解原理及实现过程的时候，复制同年或历年的代码、实验报告等内容
- **实验过程中允许相互借鉴，但是抄袭行为不会被建议，也绝对不会被允许，请自重并尊重老师的辛勤付出与协作者的努力**
- 如果你还不会配置本实验的环境，或者你在配置过程中出现了某种问题，请确保你已经阅读下文的 [**环境配置**](https://github.com/youngsamwei/DongmenDB#环境配置) 部分的内容
- 如果你确定要对你遇到的问题进行提问，请在发起 `issue` 时根据 `issue模板` 的要求，详尽的阐述你所遇到的问题、所使用的CLion程序版本、所使用的编译器具体信息等，以便其他人更好的帮助你解决问题
- 考虑到实验内容正在不断完善且本仓库内容非即时更新，若在实验时遇到老师或协作者在其他即时平台上发布的更改信息，请以你收到的官方信息为准
- 为更好地同步本仓库的内容，建议学习 `git` 的相关知识，这并不难，但会对你有很大的帮助

# 环境配置

+ 你应该确保任何涉及到本实验的软件、编译器等路径**均不包含中文字符**
+ 如果你本身的网络条件欠佳，请下载 `MinGW-w64` 的离线安装版本，即非 `Online Installer` 的其他版本

本仓库规定参与者应使用如下软件：

|软件|官网链接|备注|
|:-----|:-----|:-----|
|Git for windows|[Git-scm](https://git-scm.com/downloads)|用于版本控制同步仓库|
|CLion|[Jetbrains_CLion](https://www.jetbrains.com/clion/download/#section=windows)|[学生免费](https://www.jetbrains.com/zh/student/)|
|MinGW-w64|[Sourceforge](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/8.1.0/)|请下载下方最新的`*-posix-seh`版本|

## 视频指导

如果你没有配置过相似实验环境的基础，或在实验环境配置过程中出现了某种问题，你可以先参照下述视频内容

[Bilibili](https://www.bilibili.com/video/av66411568/?p=2)

[OneDrive](https://dddidiaoacid-my.sharepoint.com/:v:/g/personal/z105_sipan_ml/EdRgYq3h3BdJjPD3fzQCiOMBiIkzJs2TS2KIiU7cKnsdRw?e=MTfQf1)


## 离线备份

如果你无法下载上述软件，或你无法找到下载链接，你可以在这里找到 2019年实验时，使用的版本文件备份

|软件|备份地址|
|:-----|:-----|
|Git for windows|[OneDrive](https://dddidiaoacid-my.sharepoint.com/:u:/g/personal/z105_sipan_ml/EbfZ-Qif5VJEvK2DtoDN3N4BfwNsmBMtzokyhJ1DaOVnkg?e=rakC8g)|
|CLion|[OneDrive](https://dddidiaoacid-my.sharepoint.com/:u:/g/personal/z105_sipan_ml/EW9sPpMe2S1FizFYU26mV2sBNRUi_EZkFr7b-hsbavQbIQ?e=WcAkuM)|
|MinGW-w64|[OneDrive](https://dddidiaoacid-my.sharepoint.com/:u:/g/personal/z105_sipan_ml/EZk2qJQRDS9FrqzBBfsB4MQBv1BMthyd3r_kM2n8ocrZAg?e=3fPrkb)|

+ 上述备份软件确保可以顺利完成 2019年的仓库实验内容，对之后的仓库实验也应该可以完成，但不做保证

视频的离线下载地址：[OneDrive](https://dddidiaoacid-my.sharepoint.com/:f:/g/personal/z105_sipan_ml/EoYguicN4cpIkKuHfj4a-3sB28j_PWF6AWmvF8OyKY-LfQ?e=jef2l7)

# 贡献与协作

+ 本实验仍在不断地完善与成长中，所以非常欢迎各位同学为本仓库修复各种可能的错误，或是提供各种指导性的说明文档等，包括但不限于添加实验代码的相关注释、说明文档、指导文档等指导性内容
+ 如果你乐意为本仓库做出贡献，你可以联系老师获得协作者身份
+ 假如你是本实验的协作者，请悉知并遵守如下几点：
	+ 请保证每一次推送代码之前，务必先 `pull` 拉取仓库的所有改动，解决冲突合并后再提交
	+ 请确保无论使用何种方式更改本实验仓库，不包含涉及到实验答案的任何内容，包括但不限于涉及实验内容直接或间接答案的代码或其他不宜与其他课程参与者共示的文件内容
	+ 若你将要对仓库进行改动，请确保该次改动仅仅修改了错误相关的内容，且对其他实验结果没有不良影响
	+ 建议协作者知晓 `git` 与 `github` 的操作流程，在操作之前进行备份，避免错误导致仓库状态异常无法修复
