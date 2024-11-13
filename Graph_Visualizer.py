from graphviz import Source

# Load the latest DOT file content
graph = Source.from_file('graph.dot')

# Render and save the graph as a PNG
graph.render('graph', format='png', cleanup=True)  # cleanup=True removes the intermediate .dot file
graph