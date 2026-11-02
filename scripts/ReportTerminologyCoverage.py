#!/usr/bin/env python3
"""Generate an honest inventory of lesson mentions and outstanding reviews."""
import csv
import html
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'docs/terminology/coverage.md'
with (ROOT / 'docs/terminology/critical-terms.csv').open(encoding='utf-8', newline='') as stream:
    rows = list(csv.DictReader(stream))
lines = ['# 课程词条覆盖与待核清单', '',
         '本清单由脚本生成。课程计数仅匹配 HTML 中的英文词头；缩写和API名称可能不匹配。语义复核范围见逐课记录，不以匹配计数充当认证。', '',
         '## 课程词头匹配', '', '| 课程 | 匹配概念数 |', '|---|---:|']
for path in sorted((ROOT / 'lessons').glob('[0-9][0-9][0-9][0-9]-*.html')):
    body = html.unescape(re.sub(r'<[^>]+>', ' ', path.read_text(encoding='utf-8')))
    hits = sum(bool(re.search(r'(?<![A-Za-z0-9])' + re.escape(row['english']) + r'(?![A-Za-z0-9])', body, re.I)) for row in rows)
    lines.append(f'| [{path.stem}](../../lessons/{path.name}) | {hits} |')
# A human-readable inventory of the material checked so far; it is not a claim
# that every lesson paragraph and example has been reviewed.
scopes = [('第01课术语、定义与数值例子', '核对配置/编译/链接/运行时的不同语境，Debug/Release不是语言关键字。'), ('第02课术语、定义与数值例子', '核对类型/通道/步长/所有权；外部数据不由Mat自动接管。'), ('第03课术语、定义与数值例子', '核对颜色顺序与存储类型；色彩模型不等于具体色彩空间。'), ('第04课术语、定义与数值例子', '已补CDF、动态范围；区分归一化概率、计数与存储位深。'), ('第05课术语、定义与数值例子', '已补前景；前景由任务指定，不一定是亮像素。'), ('第06课术语、定义与数值例子', '核对相关、卷积及边界；非对称核用于区分方向。'), ('第07课术语、定义与数值例子', '核对开闭顺序和结构元素；默认边界非简单零填充。'), ('第08课术语、定义与数值例子', '区分边缘NMS和框NMS；双阈值作用于梯度而非原始亮度。'), ('第09课术语、定义与数值例子', '区分像素面积和轮廓多边形面积；核对层级和背景标签。'), ('第10课术语、定义与数值例子', '已补过分割；未知标记0和边界-1不同。'), ('第11课术语、定义与数值例子', '核对仿射、单应、逆映射；画布大小不由矩阵自动保证。'), ('第12课术语、定义与数值例子', '已补NCC和歧义；课程去均值NCC明确对应TM_CCOEFF_NORMED。'), ('第13课术语、定义与数值例子', '已补接缝；共识、几何配准、融合分开。'), ('第14课术语、定义与数值例子', '已补契约/退化；已知内参PnP与多视图内参标定分开。'), ('第15课术语、定义与数值例子', '核对深度与位姿、单位；零视差不返回有限深度。'), ('第16课术语、定义与数值例子', '核对帧率、帧时间戳、编码器/容器；PNG序列不代表视频验收。'), ('第17课术语、定义与数值例子', '核对光流/身份跟踪；Kalman创新门控不保证关联正确。'), ('第18课术语、定义与数值例子', '已补曝光时间和平场校正；线性模型仅用于未饱和有效增益。'), ('第19课术语、定义与数值例子', '核对亚像素、准确度、精密度与不确定度，避免亚像素即高准确度。'), ('第20课术语、定义与数值例子', '明确缺陷为正类；修正多数良品示例，区分误报/漏报。'), ('第21课术语、定义与数值例子', '已补验证集/测试集；工件分组及训练参数固定避免泄漏。'), ('第22课术语、定义与数值例子', '核对张量形状/布局、归一化及推理；SqueezeNet分数不是概率。'), ('第23课术语、定义与数值例子', '核对检测/语义分割/实例分割、NMS、逆letterbox与标签插值。'), ('第24课术语、定义与数值例子', '核对延迟、吞吐、P95及计时边界；Debug/Release不是算法对照。'), ('第25课术语、定义与数值例子', '核对背压、竞态、所有权；有界丢帧与阻塞是不同策略。'), ('第26课术语、定义与数值例子', '核对上传/推理/下载/同步及部署；未验证GPU。'), ('第27课术语、定义与数值例子', '核对相关/卷积非对称核；保留源码版本及边界配置。'), ('第28课术语、定义与数值例子', '核对版本过滤、取消、独立图像和导出原子提交；已修正同步描述。'), ('第29课术语、定义与数值例子', '核对像素尺度、公差与判定规则；人工缺陷不等于生产验收。'), ('第30课术语、定义与数值例子', '核对视频分类、光流与目标ID跟踪边界；真实模型并非检测器。')]
lines += ['', '## 逐课语境复核记录（2026-11-02）', '',
          '范围依据课程标题、术语节及已检查的代码/API；本轮逐课复核了显式术语、主要数值例子及其与词表的语境；不是对所有英文API标识符的穷尽词典，也不是权威中文译名认证。', '',
          '| 课次 | 检查范围 | 复核结论与边界 |', '|---|---|---|']
for number, (scope, gap) in enumerate(scopes, 1):
    lines.append(f'| {number:02d} | {scope} | {gap} |')
lines += ['', '## 待逐条核实', '', '下列状态由 CSV 原样列出；来源链接存在或词头匹配均不等于核校内容。', '', '| 编号 | 英文 | 来源 | 状态 |', '|---|---|---|---|']
for row in rows:
    if '待' in row['review_status']:
        lines.append(f"| {row['id']} | {row['english']} | [{row['source_title']}]({row['source_url']}) | {row['review_status']} |")
OUT.write_text('\n'.join(lines) + '\n', encoding='utf-8', newline='\n')
print(f'Generated {OUT}; pending: {sum("待" in r["review_status"] for r in rows)}')
