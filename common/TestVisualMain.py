# TestVisualMain.py
from graphviz import Digraph
import json

# 语法分析树的可视化
# Usage 把树的节点输出到 nodes.txt 中，格式为 节点编号，节点值，该节点的父节点编号
#       之后运行该Python文件即可

def visualize_tree(nodes):
    dot = Digraph(comment='Tree Structure', format='png')
    dot.attr(rankdir='TB', size='6,6')  # Top to Bottom layout, size of the graph
    dot.attr('graph', dpi='1000')  # Set the resolution to 300 dots per inch
    dot.attr('node', shape='circle', style='filled', fillcolor='skyblue', fontname='Arial', fontsize='12', fontweight='bold')
    dot.attr('edge', color='black')

    # Add nodes
    for node in nodes:
        node_id, content, _ = node
        dot.node(str(node_id), content)

    # Add edges
    for node in nodes:
        node_id, _, parent_id = node
        if parent_id != '-':
            dot.edge(str(parent_id), str(node_id))

    return dot

# 从文件中读取节点数据
def read_nodes_from_file(filename):
    nodes = []
    with open(filename, 'r') as file:
        for line in file:
            parts = line.strip().split()
            if len(parts) == 3:
                node_id, content, parent_id = parts
                nodes.append((int(node_id), content, int(parent_id) if parent_id != '-' else '-'))
    return nodes

# 从txt文件中读取节点
nodes = read_nodes_from_file('../files/nodes.txt')

# 可视化树
output_path = '../files/tree_visualization'  # Change this to your desired output path
dot = visualize_tree(nodes)
dot.render(output_path, format='png', cleanup=True)
# print(dot.source)

# 语法分析栈的可视化
# Usage 将栈的动作存入json文件中，然后用json文件的内容修改html文件

# 读取 HTML 文件
with open('../files/StackVisualization0.html', 'r', encoding='utf-8') as html_file:
    html_content = html_file.read()

# 读取 JSON 文件
with open('../files/actions.json', 'r', encoding='utf-8') as json_file:
    json_data = json.load(json_file)

# 在 HTML 内容中查找并替换 actions 数组
html_content = html_content.replace("let actions = [];", f"let actions = {json.dumps(json_data)};")

# 将修改后的 HTML 内容写回文件
with open('../files/StackVisualization.html', 'w', encoding='utf-8') as modified_html_file:
    modified_html_file.write(html_content)
