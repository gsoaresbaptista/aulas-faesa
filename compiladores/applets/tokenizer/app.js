document.addEventListener('DOMContentLoaded', () => {
    const codeInput = document.getElementById('code-input');
    const visualizer = document.getElementById('token-visualizer');
    const symbolTableBody = document.getElementById('symbol-table-body');
    const highlightingContent = document.getElementById('highlighting-content');
    const highlighting = document.getElementById('highlighting');
    const prevBtn = document.getElementById('prev-page');
    const nextBtn = document.getElementById('next-page');
    const pageInfo = document.getElementById('page-info');

    let currentSymbols = [];
    let currentPage = 1;
    const PAGE_SIZE = 4;

    // Define SLANG grammar for Prism
    if (typeof Prism !== 'undefined') {
        Prism.languages.slang = {
            'comment': [
                { pattern: /(^|[^\\])\/\*[\s\S]*?(?:\*\/|$)/, lookbehind: true, greedy: true },
                { pattern: /(^|[^\\:])\/\/.*/, lookbehind: true, greedy: true }
            ],
            'string': { pattern: /(["'])(?:\\(?:\r\n|[\s\S])|(?!\1)[^\\\r\n])*\1/, greedy: true },
            'keyword': /\b(?:if|else|loop|while|for|fn|return|true|false|int|bool|float|char|string|void|and|or|not)\b/,
            'boolean': /\b(?:true|false)\b/,
            'function': /\b\w+(?=\()/,
            'number': /\b\d+(?:\.\d+)?(?:e[+-]?\d+)?\b/i,
            'operator': /->|[-+]{1,2}|!|<=?|>=?|={1,3}|&{1,2}|\|{1,2}|\?|\*|\/|%|\^|~/,
            'punctuation': /[{}[\];(),.:]/
        };
    }

    // Minimalistic color palette for tokens (Visualizer Section)
    const typeColors = {
        [TokenType.IDENTIFIER]: '#8B5CF6',
        [TokenType.NUMBER]: '#10B981',
        [TokenType.STRING]: '#F59E0B',
        [TokenType.CHAR]: '#F59E0B',
        [TokenType.ERROR]: '#EF4444',
        'KEYWORD': '#3B82F6',
        'OPERATOR': '#6B7280',
        'DELIMITER': '#9CA3AF'
    };

    function updateHighlighting() {
        let text = codeInput.value;
        if (text[text.length - 1] === "\n") text += " ";
        highlightingContent.textContent = text;
        if (typeof Prism !== 'undefined') Prism.highlightElement(highlightingContent);
    }

    function syncScroll() {
        highlighting.scrollTop = codeInput.scrollTop;
        highlighting.scrollLeft = codeInput.scrollLeft;
    }

    function getTokenCategory(type) {
        if (type.startsWith('KW_')) return 'KEYWORD';
        if ([TokenType.LPAREN, TokenType.RPAREN, TokenType.LBRACE, TokenType.RBRACE, TokenType.COLON, TokenType.SEMICOLON, TokenType.COMMA].includes(type)) return 'DELIMITER';
        if (type === TokenType.IDENTIFIER || type === TokenType.NUMBER || type === TokenType.STRING || type === TokenType.CHAR || type === TokenType.ERROR) return type;
        return 'OPERATOR';
    }

    function updateSymbolTable() {
        symbolTableBody.innerHTML = '';
        const totalPages = Math.ceil(currentSymbols.length / PAGE_SIZE) || 1;

        if (currentPage > totalPages) currentPage = totalPages;
        if (currentPage < 1) currentPage = 1;

        const start = (currentPage - 1) * PAGE_SIZE;
        const end = start + PAGE_SIZE;
        const pageItems = currentSymbols.slice(start, end);

        pageItems.forEach(sym => {
            const tr = document.createElement('tr');
            tr.innerHTML = `<td>${sym.id}</td><td>${sym.lexeme}</td>`;
            symbolTableBody.appendChild(tr);
        });

        pageInfo.textContent = `Página ${currentPage} de ${totalPages}`;
        prevBtn.disabled = currentPage === 1;
        nextBtn.disabled = currentPage === totalPages;
    }

    prevBtn.addEventListener('click', () => {
        if (currentPage > 1) {
            currentPage--;
            updateSymbolTable();
        }
    });

    nextBtn.addEventListener('click', () => {
        const totalPages = Math.ceil(currentSymbols.length / PAGE_SIZE);
        if (currentPage < totalPages) {
            currentPage++;
            updateSymbolTable();
        }
    });

    function renderTokens(tokens) {
        visualizer.innerHTML = '';
        const symbolMap = new Map(); // Lexeme -> ID
        const newSymbols = [];

        tokens.forEach((token, index) => {
            if (token.type === TokenType.EOF) return;

            const category = getTokenCategory(token.type);
            const color = typeColors[category] || '#333';

            // Determine ID and Tooltip Content
            let tooltipContent = `<${token.type}, ${token.lexeme}>`;

            if (token.type === TokenType.IDENTIFIER) {
                let id;
                if (symbolMap.has(token.lexeme)) {
                    id = symbolMap.get(token.lexeme);
                } else {
                    id = newSymbols.length;
                    symbolMap.set(token.lexeme, id);
                    newSymbols.push({ id: id, lexeme: token.lexeme, type: token.type });
                }
                tooltipContent = `<IDENTIFIER, ${id}>`;
            } else if (token.type === TokenType.STRING) {
                tooltipContent = `<STRING, "${token.lexeme}">`;
            }

            // 1. Render Token Block
            const wrapper = document.createElement('div');
            wrapper.className = 'token-block-wrapper';
            wrapper.title = tooltipContent;

            const textSpan = document.createElement('span');
            textSpan.className = 'token-text';
            textSpan.textContent = token.lexeme;

            wrapper.appendChild(textSpan);
            visualizer.appendChild(wrapper);

            // RoughJS sketchy border
            setTimeout(() => {
                const w = wrapper.offsetWidth;
                const h = wrapper.offsetHeight;

                const canvas = document.createElement('canvas');
                canvas.className = 'token-block-canvas';
                canvas.width = w + 10;
                canvas.height = h + 10;
                canvas.style.left = '-5px';
                canvas.style.top = '-5px';

                wrapper.insertBefore(canvas, textSpan);

                const rc = rough.canvas(canvas);
                rc.rectangle(5, 5, w, h, {
                    roughness: 1.5,
                    stroke: color,
                    strokeWidth: 2,
                    seed: index
                });
            }, 0);
        });

        currentSymbols = newSymbols;
        currentPage = 1;
        updateSymbolTable();
    }

    function handleInput() {
        const source = codeInput.value;
        const lexer = new Lexer(source);
        const tokens = lexer.scanAllTokens();
        renderTokens(tokens);
        updateHighlighting();
    }

    codeInput.addEventListener('input', handleInput);
    codeInput.addEventListener('scroll', syncScroll);

    // Initial sample
    codeInput.value = `/* 
   Cálculo de Fatorial em Slang
*/

fn fatorial(n: int) -> int {
    if n <= 1 {
        return 1
    }
    return n * fatorial(n - 1)
}

fn main(args: void) -> void {
    x: int = 5
    resultado: string = "O fatorial é: "
    
    if x >= 0 and x < 13 {
        print(resultado)
        print(fatorial(x))
    } else {
        print("Erro: Entrada inválida")
    }
}`;
    handleInput();
});
