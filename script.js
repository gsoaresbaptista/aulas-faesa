/* script.js - Lógica para Acessibilidade e VLibras */

// 1. Inicialização do Widget VLibras
// Certifique-se de que a div do VLibras está no HTML
document.addEventListener('DOMContentLoaded', function() {
    if (window.VLibras) {
        new window.VLibras.Widget('https://vlibras.gov.br/app');
    }
});

// 2. Controle de Alto Contraste
(function () {
    var Contrast = {
        storage: 'contrastState',
        cssClass: 'contrast',
        update: function() {
            // Verifica o estado salvo e aplica/remove a classe no body
            var isContrast = localStorage.getItem(this.storage) === 'true';
            document.body.classList.toggle(this.cssClass, isContrast);
        }
    };

    // Função global para ser chamada pelo botão
    window.toggleContrast = function() {
        var currentState = localStorage.getItem(Contrast.storage) === 'true';
        localStorage.setItem(Contrast.storage, !currentState);
        Contrast.update();
    };

    // Inicializa ao carregar a página
    Contrast.update();
})();

// 3. Controle de Tamanho da Fonte (Vanilla JS - sem jQuery)
document.addEventListener('DOMContentLoaded', function() {
    var contentArea = document.getElementById('content-area');
    var btnAumentar = document.getElementById('aumentar-fonte');
    var btnDiminuir = document.getElementById('diminuir-fonte');

    if (contentArea && btnAumentar && btnDiminuir) {
        // Função para pegar o tamanho atual
        function getCurrentFontSize() {
            var style = window.getComputedStyle(contentArea, null).getPropertyValue('font-size');
            return parseFloat(style);
        }

        btnAumentar.addEventListener('click', function() {
            var currentSize = getCurrentFontSize();
            contentArea.style.fontSize = (currentSize + 1) + 'px';
        });

        btnDiminuir.addEventListener('click', function() {
            var currentSize = getCurrentFontSize();
            if (currentSize > 12) { // Limite mínimo
                contentArea.style.fontSize = (currentSize - 1) + 'px';
            }
        });
    }
});
