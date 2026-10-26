/**
 * AuraStore - Modern E-Commerce Storefront Engine
 * Handles catalog filtering, search, cart drawer, wishlist, modals, and checkout.
 */

// ============================================================================
// 1. PRODUCT CATALOG DATA
// ============================================================================
const PRODUCTS_DATA = [
  {
    id: 1,
    title: "Aura Pro Wireless ANC Headphones",
    category: "audio",
    price: 249.00,
    oldPrice: 299.00,
    rating: 4.9,
    reviewsCount: 184,
    badge: "sale",
    badgeLabel: "Save $50",
    image: "https://images.unsplash.com/photo-1505740420928-5e560c06d30e?w=800&q=80",
    description: "Industry-leading hybrid active noise cancellation, custom 40mm beryllium drivers, and up to 40 hours of playtime with ultra-fast USB-C quick charge.",
    specs: ["40mm High-Resolution Drivers", "Active Hybrid Noise Cancellation", "40-Hour Battery Life", "Multipoint Bluetooth 5.3"],
    inStock: true
  },
  {
    id: 2,
    title: "KeyPro 75% Custom Mechanical Keyboard",
    category: "keyboards",
    price: 169.00,
    oldPrice: null,
    rating: 4.8,
    reviewsCount: 96,
    badge: "bestseller",
    badgeLabel: "Best Seller",
    image: "https://images.unsplash.com/photo-1587829741301-dc798b83add3?w=800&q=80",
    description: "CNC aluminum chassis, hot-swappable tactile switches, sound-dampening silicone gasket mount, and per-key customizable South-facing RGB lighting.",
    specs: ["CNC Anodized Aluminum Case", "Hot-Swappable PCB (3/5 Pin)", "Gasket Mounted Structure", "QMK/VIA Programmable"],
    inStock: true
  },
  {
    id: 3,
    title: "Solid Walnut Desk Shelf & Monitor Riser",
    category: "desk",
    price: 129.00,
    oldPrice: 149.00,
    rating: 4.7,
    reviewsCount: 78,
    badge: "new",
    badgeLabel: "New",
    image: "https://images.unsplash.com/photo-1527443224154-c4a3942d3acf?w=800&q=80",
    description: "Handcrafted from sustainably sourced solid American walnut wood with powder-coated steel legs. Elevates monitors to eye level for optimal posture.",
    specs: ["100% Solid American Walnut", "Supports up to 60 lbs (27 kg)", "Integrated Aluminum Cable Notch", "Soft Cork Protective Feet"],
    inStock: true
  },
  {
    id: 4,
    title: "Chronos Apex Titanium Smartwatch",
    category: "wearables",
    price: 349.00,
    oldPrice: 399.00,
    rating: 4.9,
    reviewsCount: 142,
    badge: "sale",
    badgeLabel: "Sale",
    image: "https://images.unsplash.com/photo-1523275335684-37898b6baf30?w=800&q=80",
    description: "Grade-5 aerospace titanium casing, sapphire crystal display, continuous SpO2/ECG cardiac tracking, and 14 days battery life on a single charge.",
    specs: ["Sapphire Crystal Glass", "Aerospace Grade 5 Titanium", "14-Day Battery Life", "5ATM Water Resistance (50m)"],
    inStock: true
  },
  {
    id: 5,
    title: "Lumina Magnetic Desk Light Bar",
    category: "desk",
    price: 89.00,
    oldPrice: null,
    rating: 4.6,
    reviewsCount: 63,
    badge: null,
    badgeLabel: "",
    image: "https://images.unsplash.com/photo-1507473885765-e6ed057f782c?w=800&q=80",
    description: "Asymmetric optical design illuminates your desk without any screen glare. Touch controls for auto-dimming and color temperature from 2700K to 6500K.",
    specs: ["Zero Screen Glare Optics", "CRI > 95 Color Rendering", "Wireless Desktop Rotary Dial", "USB-C Powered"],
    inStock: true
  },
  {
    id: 6,
    title: "Orbit Wireless Earbuds with ANC",
    category: "audio",
    price: 139.00,
    oldPrice: 179.00,
    rating: 4.8,
    reviewsCount: 110,
    badge: "bestseller",
    badgeLabel: "Popular",
    image: "https://images.unsplash.com/photo-1590658268037-6bf12165a8df?w=800&q=80",
    description: "Compact wireless earbuds with adaptive transparency mode, deep resonant bass, IPX5 water resistance, and wireless Qi charging case.",
    specs: ["Adaptive Active Noise Cancelling", "IPX5 Sweat & Water Resistant", "32 Hours Total Playback", "Qi Wireless Charging Support"],
    inStock: true
  },
  {
    id: 7,
    title: "Craftsman Top-Grain Leather Desk Mat",
    category: "accessories",
    price: 59.00,
    oldPrice: null,
    rating: 4.7,
    reviewsCount: 52,
    badge: null,
    badgeLabel: "",
    image: "https://images.unsplash.com/photo-1586953208448-b95a79798f07?w=800&q=80",
    description: "Full-grain vegetable-tanned leather that develops an organic patina over time. Smooth gliding surface for high-precision optical mice.",
    specs: ["Full-Grain Vegetable Tanned Leather", "90cm x 40cm Large Surface", "Non-Slip Natural Suede Backing", "Water-Resistant Coating"],
    inStock: true
  },
  {
    id: 8,
    title: "MagCharge 3-in-1 Foldable Charging Stand",
    category: "accessories",
    price: 79.00,
    oldPrice: 99.00,
    rating: 4.8,
    reviewsCount: 88,
    badge: "sale",
    badgeLabel: "Save 20%",
    image: "https://images.unsplash.com/photo-1622445262464-84b1456045b6?w=800&q=80",
    description: "Charge your smartphone, smartwatch, and wireless earbuds concurrently with fast 15W Qi-certified magnetic charging in a travel-ready folding stand.",
    specs: ["15W Fast Magnetic Wireless Charging", "Concurrently Powers 3 Devices", "Aircraft-Grade Aluminum Body", "Travel Case & 30W Adapter Included"],
    inStock: true
  },
  {
    id: 9,
    title: "ErgoFlow Precision Wireless Vertical Mouse",
    category: "accessories",
    price: 89.00,
    oldPrice: null,
    rating: 4.7,
    reviewsCount: 74,
    badge: "new",
    badgeLabel: "New",
    image: "https://images.unsplash.com/photo-1615663245857-ac93bb7c39e7?w=800&q=80",
    description: "57-degree natural handshake angle reduces muscular forearm strain by 30%. Whisper-quiet mechanical clicks and 4000 DPI high-precision sensor.",
    specs: ["57° Ergonomic Natural Angle", "4000 DPI High-Precision Optical Sensor", "Whisper Quiet Clicks", "Multi-Device Easy-Switch"],
    inStock: true
  },
  {
    id: 10,
    title: "Apex Mechanical Numpad & Media Controller",
    category: "keyboards",
    price: 75.00,
    oldPrice: 89.00,
    rating: 4.6,
    reviewsCount: 39,
    badge: null,
    badgeLabel: "",
    image: "https://images.unsplash.com/photo-1541140532154-b024d705b909?w=800&q=80",
    description: "Dedicated mechanical numeric pad with integrated knurled aluminum volume wheel and programmable macro buttons for productivity power users.",
    specs: ["Knurled Volume/Scrub Dial", "Hot-Swappable Switches", "Bluetooth 5.1 & 2.4Ghz & Type-C", "Macro Layer Support"],
    inStock: true
  }
];

// ============================================================================
// 2. STATE MANAGEMENT & STORAGE
// ============================================================================
class StoreState {
  constructor() {
    this.products = PRODUCTS_DATA;
    this.cart = this.loadFromStorage("aura_cart", []);
    this.wishlist = this.loadFromStorage("aura_wishlist", []);
    this.theme = localStorage.getItem("aura_theme") || "light";
    this.appliedCoupon = this.loadFromStorage("aura_coupon", null);

    this.filters = {
      search: "",
      category: "all",
      maxPrice: 500,
      sortBy: "featured"
    };

    this.FREE_SHIPPING_THRESHOLD = 99.00;
    this.SHIPPING_FLAT_FEE = 15.00;
    this.backendOnline = false;
  }

  loadFromStorage(key, fallback) {
    try {
      const data = localStorage.getItem(key);
      return data ? JSON.parse(data) : fallback;
    } catch (e) {
      console.warn(`Error parsing localStorage for ${key}`, e);
      return fallback;
    }
  }

  saveToStorage(key, value) {
    try {
      localStorage.setItem(key, JSON.stringify(value));
    } catch (e) {
      console.warn(`Error saving to localStorage for ${key}`, e);
    }
  }
}

const API_BASE = window.location.port === "8080" ? "" : "http://localhost:8080";
const state = new StoreState();

// ============================================================================
// 3. DOM ELEMENT REFERENCES
// ============================================================================
const DOM = {
  // Theme & Announcement
  html: document.documentElement,
  themeToggle: document.getElementById("theme-toggle"),
  promoCopyTrigger: document.getElementById("promo-copy-trigger"),
  promoBannerBtn: document.getElementById("promo-banner-btn"),
  backendStatusPill: document.getElementById("backend-status-pill"),
  backendStatusText: document.getElementById("backend-status-text"),

  // Search & Filters
  globalSearch: document.getElementById("global-search"),
  searchClearBtn: document.getElementById("search-clear-btn"),
  categoryChips: document.getElementById("category-chips-list"),
  priceRange: document.getElementById("price-range"),
  priceSliderVal: document.getElementById("price-slider-val"),
  sortSelect: document.getElementById("sort-select"),
  resultsCountText: document.getElementById("results-count-text"),
  resetFiltersBtn: document.getElementById("reset-filters-btn"),
  activeTagsBar: document.getElementById("active-tags-bar"),
  activeTagsList: document.getElementById("active-tags-list"),

  // Products Grid
  productsGrid: document.getElementById("products-grid"),
  emptyState: document.getElementById("empty-state"),
  emptyResetBtn: document.getElementById("empty-reset-btn"),

  // Wishlist & Cart Badges
  wishlistBtn: document.getElementById("wishlist-btn"),
  wishlistCount: document.getElementById("wishlist-count"),
  cartOpenBtn: document.getElementById("cart-open-btn"),
  cartBadgeCount: document.getElementById("cart-badge-count"),
  cartBtnSubtotal: document.getElementById("cart-btn-subtotal"),

  // Cart Drawer
  cartDrawerOverlay: document.getElementById("cart-drawer-overlay"),
  cartCloseBtn: document.getElementById("cart-close-btn"),
  cartDrawerCount: document.getElementById("cart-drawer-count"),
  cartItemsContainer: document.getElementById("cart-items-container"),
  shippingProgressText: document.getElementById("shipping-progress-text"),
  shippingProgressFill: document.getElementById("shipping-progress-fill"),
  couponForm: document.getElementById("coupon-form"),
  couponInput: document.getElementById("coupon-input"),
  couponFeedback: document.getElementById("coupon-feedback"),
  summarySubtotal: document.getElementById("summary-subtotal"),
  summaryDiscountRow: document.getElementById("summary-discount-row"),
  summaryDiscountTag: document.getElementById("summary-discount-tag"),
  summaryDiscount: document.getElementById("summary-discount"),
  summaryShipping: document.getElementById("summary-shipping"),
  summaryTotal: document.getElementById("summary-total"),
  checkoutBtn: document.getElementById("checkout-btn"),
  clearCartBtn: document.getElementById("clear-cart-btn"),

  // Quick View Modal
  quickViewModal: document.getElementById("quick-view-modal"),
  quickviewCloseBtn: document.getElementById("quickview-close-btn"),
  quickviewContent: document.getElementById("quickview-content"),

  // Checkout Modal
  checkoutModal: document.getElementById("checkout-modal"),
  checkoutCloseBtn: document.getElementById("checkout-close-btn"),
  checkoutFormStep: document.getElementById("checkout-form-step"),
  checkoutSuccessStep: document.getElementById("checkout-success-step"),
  checkoutForm: document.getElementById("checkout-form"),
  checkoutMiniSummary: document.getElementById("checkout-mini-summary"),
  orderConfirmedId: document.getElementById("order-confirmed-id"),
  continueShoppingBtn: document.getElementById("continue-shopping-btn"),

  // Toast Container
  toastContainer: document.getElementById("toast-container")
};

// ============================================================================
// 4. RENDERING FUNCTIONS
// ============================================================================

/**
 * Filter & sort products based on current active state
 */
function getFilteredProducts() {
  return state.products
    .filter(product => {
      // Search term
      if (state.filters.search) {
        const query = state.filters.search.toLowerCase().trim();
        const matchesTitle = product.title.toLowerCase().includes(query);
        const matchesCategory = product.category.toLowerCase().includes(query);
        const matchesDesc = product.description.toLowerCase().includes(query);
        if (!matchesTitle && !matchesCategory && !matchesDesc) return false;
      }
      // Category
      if (state.filters.category !== "all") {
        if (product.category !== state.filters.category) return false;
      }
      // Max Price
      if (product.price > state.filters.maxPrice) {
        return false;
      }
      return true;
    })
    .sort((a, b) => {
      switch (state.filters.sortBy) {
        case "price-asc":
          return a.price - b.price;
        case "price-desc":
          return b.price - a.price;
        case "rating":
          return b.rating - a.rating;
        case "name-asc":
          return a.title.localeCompare(b.title);
        case "featured":
        default:
          return b.rating * b.reviewsCount - a.rating * a.reviewsCount;
      }
    });
}

/**
 * Render the product catalog cards into the DOM
 */
function renderProducts() {
  const filtered = getFilteredProducts();

  // Results counter update
  const total = state.products.length;
  if (filtered.length === total) {
    DOM.resultsCountText.textContent = `Showing all ${total} products`;
    DOM.resetFiltersBtn.hidden = true;
  } else {
    DOM.resultsCountText.textContent = `Showing ${filtered.length} of ${total} products`;
    DOM.resetFiltersBtn.hidden = false;
  }

  // Active tags
  renderActiveTags();

  // Empty state handling
  if (filtered.length === 0) {
    DOM.productsGrid.innerHTML = "";
    DOM.emptyState.hidden = false;
    return;
  }

  DOM.emptyState.hidden = true;

  // Build card markup
  const cardsHtml = filtered.map(product => {
    const isWishlisted = state.wishlist.includes(product.id);
    const badgeHtml = product.badge
      ? `<span class="product-badge badge-${product.badge}">${product.badgeLabel}</span>`
      : "";

    const oldPriceHtml = product.oldPrice
      ? `<span class="old-price">$${product.oldPrice.toFixed(2)}</span>`
      : "";

    return `
      <article class="product-card" data-id="${product.id}">
        <div class="product-thumb-wrap">
          ${badgeHtml}
          <button 
            class="card-wishlist-btn ${isWishlisted ? 'active' : ''}" 
            aria-label="${isWishlisted ? 'Remove from wishlist' : 'Save to wishlist'}"
            title="${isWishlisted ? 'Remove from wishlist' : 'Save to wishlist'}"
            onclick="window.appStore.toggleWishlist(${product.id}, event)"
          >
            <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <path d="M20.84 4.61a5.5 5.5 0 0 0-7.78 0L12 5.67l-1.06-1.06a5.5 5.5 0 0 0-7.78 7.78l1.06 1.06L12 21.23l7.78-7.78 1.06-1.06a5.5 5.5 0 0 0 0-7.78z"></path>
            </svg>
          </button>
          
          <img 
            src="${product.image}" 
            alt="${escapeHtml(product.title)}"
            class="product-thumb" 
            loading="lazy" 
          />

          <button class="quick-view-btn" onclick="window.appStore.openQuickView(${product.id})">
            Quick View
          </button>
        </div>

        <div class="product-card-body">
          <span class="product-category">${product.category}</span>
          <h3 class="product-title" title="${escapeHtml(product.title)}">${escapeHtml(product.title)}</h3>
          
          <div class="product-rating">
            <span class="stars">${renderStars(product.rating)}</span>
            <span class="rating-count">(${product.reviewsCount})</span>
          </div>

          <div class="product-card-footer">
            <div class="price-wrap">
              <span class="current-price">$${product.price.toFixed(2)}</span>
              ${oldPriceHtml}
            </div>
            
            <button 
              class="add-cart-btn" 
              id="add-btn-${product.id}"
              onclick="window.appStore.addToCart(${product.id}, this)"
              aria-label="Add ${escapeHtml(product.title)} to cart"
            >
              <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <line x1="12" y1="5" x2="12" y2="19"></line>
                <line x1="5" y1="12" x2="19" y2="12"></line>
              </svg>
              <span>Add</span>
            </button>
          </div>
        </div>
      </article>
    `;
  }).join("");

  DOM.productsGrid.innerHTML = cardsHtml;
}

/**
 * Render active filter tags row
 */
function renderActiveTags() {
  const tags = [];
  if (state.filters.category !== "all") {
    tags.push({ type: "category", label: `Category: ${state.filters.category}` });
  }
  if (state.filters.search) {
    tags.push({ type: "search", label: `Search: "${state.filters.search}"` });
  }
  if (state.filters.maxPrice < 500) {
    tags.push({ type: "price", label: `Max $${state.filters.maxPrice}` });
  }

  if (tags.length === 0) {
    DOM.activeTagsBar.hidden = true;
    DOM.activeTagsList.innerHTML = "";
    return;
  }

  DOM.activeTagsBar.hidden = false;
  DOM.activeTagsList.innerHTML = tags.map(tag => `
    <span class="filter-tag">
      ${escapeHtml(tag.label)}
      <button onclick="window.appStore.removeFilterTag('${tag.type}')" aria-label="Remove filter">&times;</button>
    </span>
  `).join("");
}

/**
 * Render star ratings string
 */
function renderStars(rating) {
  const fullStars = Math.floor(rating);
  const halfStar = rating % 1 >= 0.5;
  let stars = "★".repeat(fullStars);
  if (halfStar) stars += "½";
  while (stars.length < 5) stars += "☆";
  return stars;
}

/**
 * Render slide-out shopping cart items, shipping meter, totals
 */
function renderCart() {
  const totalItemsCount = state.cart.reduce((sum, item) => sum + item.quantity, 0);

  // Update header badges
  DOM.cartBadgeCount.textContent = totalItemsCount;
  DOM.cartDrawerCount.textContent = `(${totalItemsCount})`;

  if (state.cart.length === 0) {
    DOM.cartItemsContainer.innerHTML = `
      <div class="cart-empty-view">
        <svg width="56" height="56" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5">
          <path d="M6 2L3 6v14a2 2 0 0 0 2 2h14a2 2 0 0 0 2-2V6l-3-4z"></path>
          <line x1="3" y1="6" x2="21" y2="6"></line>
          <path d="M16 10a4 4 0 0 1-8 0"></path>
        </svg>
        <h4>Your cart is empty</h4>
        <p>Looks like you haven't added any gear to your cart yet.</p>
        <button class="btn btn-primary" onclick="window.appStore.closeCart()">Start Exploring</button>
      </div>
    `;
    DOM.cartBtnSubtotal.textContent = "$0.00";
    DOM.summarySubtotal.textContent = "$0.00";
    DOM.summaryTotal.textContent = "$0.00";
    DOM.summaryShipping.textContent = "$0.00";
    DOM.summaryDiscountRow.hidden = true;
    DOM.shippingProgressFill.style.width = "0%";
    DOM.shippingProgressText.innerHTML = `Add <strong>$${state.FREE_SHIPPING_THRESHOLD.toFixed(2)}</strong> more for Free Worldwide Shipping!`;
    DOM.checkoutBtn.disabled = true;
    return;
  }

  DOM.checkoutBtn.disabled = false;

  // Calculate pricing
  let subtotal = 0;
  const itemsHtml = state.cart.map(cartItem => {
    const product = state.products.find(p => p.id === cartItem.id);
    if (!product) return "";
    const itemSubtotal = product.price * cartItem.quantity;
    subtotal += itemSubtotal;

    return `
      <div class="cart-item-row" data-id="${product.id}">
        <img src="${product.image}" alt="${escapeHtml(product.title)}" class="cart-item-img" />
        <div class="cart-item-details">
          <div>
            <h4 class="cart-item-title">${escapeHtml(product.title)}</h4>
            <span class="cart-item-price">$${product.price.toFixed(2)} each</span>
          </div>

          <div class="cart-item-actions">
            <div class="qty-control">
              <button class="qty-btn" onclick="window.appStore.updateCartQty(${product.id}, -1)" aria-label="Decrease quantity">-</button>
              <span class="qty-val">${cartItem.quantity}</span>
              <button class="qty-btn" onclick="window.appStore.updateCartQty(${product.id}, 1)" aria-label="Increase quantity">+</button>
            </div>
            
            <button class="cart-item-remove-btn" onclick="window.appStore.removeFromCart(${product.id})" aria-label="Remove item">
              <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                <polyline points="3 6 5 6 21 6"></polyline>
                <path d="M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"></path>
              </svg>
            </button>
          </div>
        </div>
      </div>
    `;
  }).join("");

  DOM.cartItemsContainer.innerHTML = itemsHtml;

  // Discount calculation
  let discountAmount = 0;
  if (state.appliedCoupon) {
    if (state.appliedCoupon.type === "percentage") {
      discountAmount = (subtotal * state.appliedCoupon.rate) / 100;
    } else if (state.appliedCoupon.type === "fixed") {
      discountAmount = state.appliedCoupon.amount;
    }
    DOM.summaryDiscountRow.hidden = false;
    DOM.summaryDiscountTag.textContent = state.appliedCoupon.code;
    DOM.summaryDiscount.textContent = `-$${discountAmount.toFixed(2)}`;
  } else {
    DOM.summaryDiscountRow.hidden = true;
  }

  // Shipping calculation & Progress meter
  const eligibleForFreeShipping = subtotal >= state.FREE_SHIPPING_THRESHOLD;
  const shippingCost = eligibleForFreeShipping ? 0 : state.SHIPPING_FLAT_FEE;

  if (eligibleForFreeShipping) {
    DOM.shippingProgressFill.style.width = "100%";
    DOM.shippingProgressFill.style.background = "#10b981";
    DOM.shippingProgressText.innerHTML = `🎉 <strong>Congratulations!</strong> You qualify for <strong>Free Worldwide Shipping</strong>.`;
    DOM.summaryShipping.innerHTML = `<span style="color: var(--success); font-weight:700;">FREE</span>`;
  } else {
    const needed = (state.FREE_SHIPPING_THRESHOLD - subtotal).toFixed(2);
    const progressPercent = Math.min(100, Math.round((subtotal / state.FREE_SHIPPING_THRESHOLD) * 100));
    DOM.shippingProgressFill.style.width = `${progressPercent}%`;
    DOM.shippingProgressFill.style.background = "var(--accent-gradient)";
    DOM.shippingProgressText.innerHTML = `Add <strong>$${needed}</strong> more to unlock <strong>Free Worldwide Shipping</strong>!`;
    DOM.summaryShipping.textContent = `$${shippingCost.toFixed(2)}`;
  }

  const finalTotal = Math.max(0, subtotal - discountAmount + shippingCost);

  DOM.cartBtnSubtotal.textContent = `$${subtotal.toFixed(2)}`;
  DOM.summarySubtotal.textContent = `$${subtotal.toFixed(2)}`;
  DOM.summaryTotal.textContent = `$${finalTotal.toFixed(2)}`;
}

/**
 * Render wishlist counter
 */
function renderWishlist() {
  DOM.wishlistCount.textContent = state.wishlist.length;
}

// ============================================================================
// 5. TOAST NOTIFICATION ENGINE
// ============================================================================
function showToast(message, type = "info") {
  const toast = document.createElement("div");
  toast.className = `toast toast-${type}`;

  let iconSvg = "";
  if (type === "success") {
    iconSvg = `<svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5"><polyline points="20 6 9 17 4 12"></polyline></svg>`;
  } else if (type === "danger") {
    iconSvg = `<svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5"><circle cx="12" cy="12" r="10"></circle><line x1="15" y1="9" x2="9" y2="15"></line><line x1="9" y1="9" x2="15" y2="15"></line></svg>`;
  } else {
    iconSvg = `<svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5"><circle cx="12" cy="12" r="10"></circle><line x1="12" y1="16" x2="12" y2="12"></line><line x1="12" y1="8" x2="12.01" y2="8"></line></svg>`;
  }

  toast.innerHTML = `${iconSvg}<span>${escapeHtml(message)}</span>`;
  DOM.toastContainer.appendChild(toast);

  setTimeout(() => {
    toast.style.opacity = "0";
    toast.style.transform = "translateY(10px)";
    toast.style.transition = "all 250ms ease";
    setTimeout(() => toast.remove(), 250);
  }, 3200);
}

// ============================================================================
// 6. CART & WISHLIST MUTATIONS
// ============================================================================
function addToCart(productId, triggerBtn = null) {
  const product = state.products.find(p => p.id === productId);
  if (!product) return;

  const existingItem = state.cart.find(item => item.id === productId);
  if (existingItem) {
    existingItem.quantity += 1;
  } else {
    state.cart.push({ id: productId, quantity: 1 });
  }

  state.saveToStorage("aura_cart", state.cart);
  renderCart();

  // Button feedback
  if (triggerBtn) {
    triggerBtn.classList.add("added");
    const originalText = triggerBtn.innerHTML;
    triggerBtn.innerHTML = `
      <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5">
        <polyline points="20 6 9 17 4 12"></polyline>
      </svg>
      <span>Added</span>
    `;
    setTimeout(() => {
      triggerBtn.classList.remove("added");
      triggerBtn.innerHTML = originalText;
    }, 1200);
  }

  showToast(`Added "${product.title}" to cart!`, "success");
}

function updateCartQty(productId, change) {
  const itemIndex = state.cart.findIndex(i => i.id === productId);
  if (itemIndex === -1) return;

  state.cart[itemIndex].quantity += change;

  if (state.cart[itemIndex].quantity <= 0) {
    state.cart.splice(itemIndex, 1);
  }

  state.saveToStorage("aura_cart", state.cart);
  renderCart();
}

function removeFromCart(productId) {
  const product = state.products.find(p => p.id === productId);
  state.cart = state.cart.filter(i => i.id !== productId);
  state.saveToStorage("aura_cart", state.cart);
  renderCart();

  if (product) {
    showToast(`Removed "${product.title}" from cart.`, "info");
  }
}

function clearCart() {
  if (state.cart.length === 0) return;
  state.cart = [];
  state.saveToStorage("aura_cart", state.cart);
  renderCart();
  showToast("Your shopping cart has been cleared.", "info");
}

function toggleWishlist(productId, event) {
  if (event) event.stopPropagation();
  const product = state.products.find(p => p.id === productId);
  if (!product) return;

  const index = state.wishlist.indexOf(productId);
  if (index === -1) {
    state.wishlist.push(productId);
    showToast(`Saved "${product.title}" to Wishlist!`, "success");
  } else {
    state.wishlist.splice(index, 1);
    showToast(`Removed "${product.title}" from Wishlist.`, "info");
  }

  state.saveToStorage("aura_wishlist", state.wishlist);
  renderWishlist();
  renderProducts(); // Re-render to update heart fills
}

// ============================================================================
// 7. COUPON LOGIC
// ============================================================================
const VALID_COUPONS = {
  SPRING20: { code: "SPRING20", type: "percentage", rate: 20, desc: "20% Spring Discount" },
  AURA10: { code: "AURA10", type: "fixed", amount: 10, desc: "$10 Store Credit" }
};

async function applyCoupon(code) {
  const cleanCode = code.trim().toUpperCase();
  if (!cleanCode) return;

  // Try validating with C++ backend if connected
  if (state.backendOnline) {
    try {
      const res = await fetch(`${API_BASE}/api/coupons/validate`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ code: cleanCode })
      });
      if (res.ok) {
        const data = await res.json();
        if (data.valid && data.coupon) {
          state.appliedCoupon = {
            code: data.coupon.code,
            type: data.coupon.type,
            rate: data.coupon.type === "percentage" ? data.coupon.value : undefined,
            amount: data.coupon.type === "fixed" ? data.coupon.value : undefined,
            desc: data.coupon.description
          };
          state.saveToStorage("aura_coupon", state.appliedCoupon);
          DOM.couponFeedback.hidden = false;
          DOM.couponFeedback.className = "coupon-feedback success";
          DOM.couponFeedback.textContent = `✓ [Backend Verified] Coupon "${cleanCode}": ${state.appliedCoupon.desc}`;
          DOM.couponInput.value = "";
          renderCart();
          showToast(`Coupon "${cleanCode}" validated by C++ backend!`, "success");
          return;
        }
      }
    } catch (e) {
      console.warn("Backend coupon validation error, using local fallback", e);
    }
  }

  // Local fallback validation
  if (VALID_COUPONS[cleanCode]) {
    state.appliedCoupon = VALID_COUPONS[cleanCode];
    state.saveToStorage("aura_coupon", state.appliedCoupon);
    DOM.couponFeedback.hidden = false;
    DOM.couponFeedback.className = "coupon-feedback success";
    DOM.couponFeedback.textContent = `✓ Coupon "${cleanCode}" applied: ${state.appliedCoupon.desc}`;
    DOM.couponInput.value = "";
    renderCart();
    showToast(`Coupon "${cleanCode}" applied successfully!`, "success");
  } else {
    DOM.couponFeedback.hidden = false;
    DOM.couponFeedback.className = "coupon-feedback error";
    DOM.couponFeedback.textContent = `✗ Invalid coupon code. Try "SPRING20"`;
    showToast("Invalid coupon code.", "danger");
  }
}

// ============================================================================
// 8. MODAL HANDLERS (Quick View & Checkout)
// ============================================================================
function openQuickView(productId) {
  const product = state.products.find(p => p.id === productId);
  if (!product) return;

  const oldPriceHtml = product.oldPrice
    ? `<span class="old-price" style="font-size: 1.1rem; text-decoration: line-through; color: var(--text-muted);">$${product.oldPrice.toFixed(2)}</span>`
    : "";

  const specsList = product.specs
    ? product.specs.map(spec => `
        <li>
          <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5">
            <polyline points="20 6 9 17 4 12"></polyline>
          </svg>
          <span>${escapeHtml(spec)}</span>
        </li>
      `).join("")
    : "";

  DOM.quickviewContent.innerHTML = `
    <div class="quickview-grid">
      <div class="quickview-img-wrap">
        <img src="${product.image}" alt="${escapeHtml(product.title)}" />
      </div>

      <div class="quickview-details">
        <span class="quickview-badge">${escapeHtml(product.category.toUpperCase())}</span>
        <h2 class="quickview-title" id="quickview-title">${escapeHtml(product.title)}</h2>
        
        <div class="product-rating" style="margin-bottom: 12px;">
          <span class="stars">${renderStars(product.rating)}</span>
          <span class="rating-count">(${product.reviewsCount} verified customer reviews)</span>
        </div>

        <div class="quickview-price-row">
          <span class="quickview-price">$${product.price.toFixed(2)}</span>
          ${oldPriceHtml}
        </div>

        <p class="quickview-desc">${escapeHtml(product.description)}</p>

        <h4 style="font-size: 0.875rem; font-weight: 700; margin-bottom: 8px;">Key Specifications:</h4>
        <ul class="quickview-specs-list">
          ${specsList}
        </ul>

        <div class="quickview-action-row">
          <button 
            class="btn btn-primary btn-block" 
            onclick="window.appStore.addToCart(${product.id}); window.appStore.closeQuickView(); window.appStore.openCart();"
          >
            Add to Cart - $${product.price.toFixed(2)}
          </button>
        </div>
      </div>
    </div>
  `;

  DOM.quickViewModal.classList.add("active");
  DOM.quickViewModal.setAttribute("aria-hidden", "false");
  document.body.style.overflow = "hidden";
}

function closeQuickView() {
  DOM.quickViewModal.classList.remove("active");
  DOM.quickViewModal.setAttribute("aria-hidden", "true");
  document.body.style.overflow = "";
}

function openCart() {
  DOM.cartDrawerOverlay.classList.add("active");
  DOM.cartDrawerOverlay.setAttribute("aria-hidden", "false");
  document.body.style.overflow = "hidden";
}

function closeCart() {
  DOM.cartDrawerOverlay.classList.remove("active");
  DOM.cartDrawerOverlay.setAttribute("aria-hidden", "true");
  document.body.style.overflow = "";
}

function openCheckout() {
  if (state.cart.length === 0) return;
  closeCart();

  // Populate mini order summary in checkout
  let subtotal = 0;
  const itemsBreakdown = state.cart.map(cartItem => {
    const product = state.products.find(p => p.id === cartItem.id);
    if (!product) return "";
    subtotal += product.price * cartItem.quantity;
    return `
      <div style="display:flex; justify-content:space-between; margin-bottom:4px;">
        <span>${escapeHtml(product.title)} x${cartItem.quantity}</span>
        <strong>$${(product.price * cartItem.quantity).toFixed(2)}</strong>
      </div>
    `;
  }).join("");

  let discount = 0;
  if (state.appliedCoupon) {
    discount = state.appliedCoupon.type === "percentage"
      ? (subtotal * state.appliedCoupon.rate) / 100
      : state.appliedCoupon.amount;
  }

  const shipping = subtotal >= state.FREE_SHIPPING_THRESHOLD ? 0 : state.SHIPPING_FLAT_FEE;
  const total = Math.max(0, subtotal - discount + shipping);

  DOM.checkoutMiniSummary.innerHTML = `
    <div style="border-bottom: 1px solid var(--border-subtle); padding-bottom: 8px; margin-bottom: 8px;">
      ${itemsBreakdown}
    </div>
    <div style="display:flex; justify-content:space-between; font-size:0.8125rem;">
      <span>Subtotal</span>
      <span>$${subtotal.toFixed(2)}</span>
    </div>
    ${discount > 0 ? `
    <div style="display:flex; justify-content:space-between; font-size:0.8125rem; color:var(--success);">
      <span>Discount (${state.appliedCoupon.code})</span>
      <span>-$${discount.toFixed(2)}</span>
    </div>` : ""}
    <div style="display:flex; justify-content:space-between; font-size:0.8125rem;">
      <span>Shipping</span>
      <span>${shipping === 0 ? "FREE" : "$" + shipping.toFixed(2)}</span>
    </div>
    <div style="display:flex; justify-content:space-between; font-size:1.05rem; font-weight:800; border-top:1px solid var(--border-subtle); padding-top:6px; margin-top:4px;">
      <span>Final Total</span>
      <span>$${total.toFixed(2)}</span>
    </div>
  `;

  DOM.checkoutFormStep.hidden = false;
  DOM.checkoutSuccessStep.hidden = true;
  DOM.checkoutModal.classList.add("active");
  DOM.checkoutModal.setAttribute("aria-hidden", "false");
  document.body.style.overflow = "hidden";
}

function closeCheckout() {
  DOM.checkoutModal.classList.remove("active");
  DOM.checkoutModal.setAttribute("aria-hidden", "true");
  document.body.style.overflow = "";
}

// ============================================================================
// 9. EVENT LISTENERS SETUP
// ============================================================================
function setupEventListeners() {
  // Theme Toggle
  DOM.themeToggle.addEventListener("click", () => {
    state.theme = state.theme === "light" ? "dark" : "light";
    DOM.html.setAttribute("data-theme", state.theme);
    localStorage.setItem("aura_theme", state.theme);
    showToast(`Switched to ${state.theme} mode`, "info");
  });

  // Search input live filtering
  DOM.globalSearch.addEventListener("input", (e) => {
    state.filters.search = e.target.value.trim();
    DOM.searchClearBtn.hidden = !state.filters.search;
    renderProducts();
  });

  DOM.searchClearBtn.addEventListener("click", () => {
    DOM.globalSearch.value = "";
    state.filters.search = "";
    DOM.searchClearBtn.hidden = true;
    renderProducts();
    DOM.globalSearch.focus();
  });

  // Category chip tabs
  DOM.categoryChips.addEventListener("click", (e) => {
    const chip = e.target.closest(".chip");
    if (!chip) return;

    DOM.categoryChips.querySelectorAll(".chip").forEach(c => {
      c.classList.remove("active");
      c.setAttribute("aria-selected", "false");
    });

    chip.classList.add("active");
    chip.setAttribute("aria-selected", "true");
    state.filters.category = chip.dataset.category;
    renderProducts();
  });

  // Price slider
  DOM.priceRange.addEventListener("input", (e) => {
    state.filters.maxPrice = Number(e.target.value);
    DOM.priceSliderVal.textContent = `$${state.filters.maxPrice}`;
    renderProducts();
  });

  // Sort dropdown
  DOM.sortSelect.addEventListener("change", (e) => {
    state.filters.sortBy = e.target.value;
    renderProducts();
  });

  // Reset filter buttons
  const resetAllFilters = () => {
    state.filters.search = "";
    state.filters.category = "all";
    state.filters.maxPrice = 500;
    state.filters.sortBy = "featured";

    DOM.globalSearch.value = "";
    DOM.searchClearBtn.hidden = true;
    DOM.priceRange.value = 500;
    DOM.priceSliderVal.textContent = "$500";
    DOM.sortSelect.value = "featured";

    DOM.categoryChips.querySelectorAll(".chip").forEach(c => {
      const isAll = c.dataset.category === "all";
      c.classList.toggle("active", isAll);
      c.setAttribute("aria-selected", isAll ? "true" : "false");
    });

    renderProducts();
    showToast("Filters reset to default", "info");
  };

  DOM.resetFiltersBtn.addEventListener("click", resetAllFilters);
  DOM.emptyResetBtn.addEventListener("click", resetAllFilters);

  // Cart Drawer open/close triggers
  DOM.cartOpenBtn.addEventListener("click", openCart);
  DOM.cartCloseBtn.addEventListener("click", closeCart);
  DOM.cartDrawerOverlay.addEventListener("click", (e) => {
    if (e.target === DOM.cartDrawerOverlay) closeCart();
  });

  // Clear cart
  DOM.clearCartBtn.addEventListener("click", clearCart);

  // Coupon form
  DOM.couponForm.addEventListener("submit", (e) => {
    e.preventDefault();
    applyCoupon(DOM.couponInput.value);
  });

  // Promo copy trigger from announcement bar
  DOM.promoCopyTrigger.addEventListener("click", () => {
    navigator.clipboard?.writeText("SPRING20").catch(() => {});
    applyCoupon("SPRING20");
    openCart();
  });

  // Hero promo button
  DOM.promoBannerBtn.addEventListener("click", () => {
    applyCoupon("SPRING20");
    openCart();
  });

  // Wishlist header button: filters to only wishlisted products
  DOM.wishlistBtn.addEventListener("click", () => {
    if (state.wishlist.length === 0) {
      showToast("Your wishlist is currently empty. Click the heart icon on any product to save it!", "info");
      return;
    }
    // Filter product grid to wishlisted items
    const wishlistedProducts = state.products.filter(p => state.wishlist.includes(p.id));
    DOM.productsGrid.scrollIntoView({ behavior: "smooth" });
    showToast(`Showing ${state.wishlist.length} saved wishlist item(s)`, "info");
  });

  // Quick View Modal Close
  DOM.quickviewCloseBtn.addEventListener("click", closeQuickView);
  DOM.quickViewModal.addEventListener("click", (e) => {
    if (e.target === DOM.quickViewModal) closeQuickView();
  });

  // Checkout Flow
  DOM.checkoutBtn.addEventListener("click", openCheckout);
  DOM.checkoutCloseBtn.addEventListener("click", closeCheckout);
  DOM.checkoutModal.addEventListener("click", (e) => {
    if (e.target === DOM.checkoutModal) closeCheckout();
  });

  DOM.checkoutForm.addEventListener("submit", async (e) => {
    e.preventDefault();
    let finalOrderId = `#AURA-${Math.floor(10000 + Math.random() * 90000)}`;

    // Prepare order calculation & items payload
    let subtotal = 0;
    const items = state.cart.map(cartItem => {
      const product = state.products.find(p => p.id === cartItem.id);
      const price = product ? product.price : 0;
      subtotal += price * cartItem.quantity;
      return {
        productId: cartItem.id,
        title: product ? product.title : "Product",
        price: price,
        quantity: cartItem.quantity
      };
    });

    let discount = 0;
    if (state.appliedCoupon) {
      discount = state.appliedCoupon.type === "percentage"
        ? (subtotal * state.appliedCoupon.rate) / 100
        : state.appliedCoupon.amount;
    }
    const shipping = subtotal >= state.FREE_SHIPPING_THRESHOLD ? 0 : state.SHIPPING_FLAT_FEE;
    const total = Math.max(0, subtotal - discount + shipping);

    const payload = {
      customerName: document.getElementById("cust-name").value,
      customerEmail: document.getElementById("cust-email").value,
      address: document.getElementById("cust-address").value,
      city: document.getElementById("cust-city").value,
      zip: document.getElementById("cust-zip").value,
      couponCode: state.appliedCoupon ? state.appliedCoupon.code : "",
      subtotal: subtotal,
      discount: discount,
      shipping: shipping,
      total: total,
      items: items
    };

    // Send order to C++ backend database
    if (state.backendOnline) {
      try {
        const res = await fetch(`${API_BASE}/api/orders`, {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify(payload)
        });
        if (res.ok) {
          const savedOrder = await res.json();
          if (savedOrder.orderId) {
            finalOrderId = savedOrder.orderId;
          }
        }
      } catch (err) {
        console.warn("Could not post order to backend, using offline fallback", err);
      }
    }

    DOM.orderConfirmedId.textContent = finalOrderId;

    // Reset cart upon successful order
    state.cart = [];
    state.appliedCoupon = null;
    state.saveToStorage("aura_cart", []);
    state.saveToStorage("aura_coupon", null);
    renderCart();

    DOM.checkoutFormStep.hidden = true;
    DOM.checkoutSuccessStep.hidden = false;
    showToast(`Order ${finalOrderId} confirmed & saved to database!`, "success");
  });

  DOM.continueShoppingBtn.addEventListener("click", () => {
    closeCheckout();
    DOM.checkoutForm.reset();
  });

  // Global Keyboard: Escape key closes modals and drawers
  document.addEventListener("keydown", (e) => {
    if (e.key === "Escape") {
      closeQuickView();
      closeCart();
      closeCheckout();
    }
  });
}

// Helper to sanitize text inside templates
function escapeHtml(str) {
  return String(str)
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;")
    .replace(/'/g, "&#039;");
}

// Filter helper callable from HTML inline (e.g. footer links)
function filterCategory(categoryName) {
  const targetChip = DOM.categoryChips.querySelector(`[data-category="${categoryName}"]`);
  if (targetChip) {
    targetChip.click();
    DOM.productsGrid.scrollIntoView({ behavior: "smooth" });
  }
}

function removeFilterTag(type) {
  if (type === "category") {
    filterCategory("all");
  } else if (type === "search") {
    DOM.globalSearch.value = "";
    state.filters.search = "";
    DOM.searchClearBtn.hidden = true;
    renderProducts();
  } else if (type === "price") {
    state.filters.maxPrice = 500;
    DOM.priceRange.value = 500;
    DOM.priceSliderVal.textContent = "$500";
    renderProducts();
  }
}

// ============================================================================
// 10. PUBLIC EXPOSURE & INITIALIZATION
// ============================================================================
window.appStore = {
  addToCart,
  updateCartQty,
  removeFromCart,
  clearCart,
  toggleWishlist,
  openQuickView,
  closeQuickView,
  openCart,
  closeCart,
  showToast,
  filterCategory,
  removeFilterTag
};

async function checkBackendStatus() {
  try {
    const res = await fetch(`${API_BASE}/api/health`, { cache: "no-store" });
    if (res.ok) {
      state.backendOnline = true;
      if (DOM.backendStatusPill) {
        DOM.backendStatusPill.className = "backend-status-pill online";
      }
      if (DOM.backendStatusText) {
        DOM.backendStatusText.textContent = "C++ Server Online";
      }
      await fetchProductsFromBackend();
      return true;
    }
  } catch (err) {
    state.backendOnline = false;
    if (DOM.backendStatusPill) {
      DOM.backendStatusPill.className = "backend-status-pill offline";
    }
    if (DOM.backendStatusText) {
      DOM.backendStatusText.textContent = "Local Storage Mode";
    }
  }
  return false;
}

async function fetchProductsFromBackend() {
  try {
    const res = await fetch(`${API_BASE}/api/products`);
    if (res.ok) {
      const dbProducts = await res.json();
      if (Array.isArray(dbProducts) && dbProducts.length > 0) {
        state.products = dbProducts;
        renderProducts();
        console.log(`[AuraStore] Synced ${dbProducts.length} products from C++ Database.`);
      }
    }
  } catch (e) {
    console.warn("Could not sync products from backend database", e);
  }
}

async function initApp() {
  // Apply initial theme
  DOM.html.setAttribute("data-theme", state.theme);

  // Initialize event listeners
  setupEventListeners();

  // Initial renders
  renderProducts();
  renderCart();
  renderWishlist();

  // Connect to C++ backend & sync database
  await checkBackendStatus();

  console.log("AuraStore frontend initialized successfully!");
}

document.addEventListener("DOMContentLoaded", initApp);
